#include "MotorDriver.h"
#include "LineSensing.h"
#include "ColourSensing.h"
#include "DistanceSensing.h"
#include "SerialComs.h"

#include <Wire.h>

#define DISTANCE_ATTEMPTS 65
#define STOP_DISTANCE 3
#define MAX_OBJECT_DISTANCE 78
#define RETURN_FACTOR 0.9 //0.9
#define ARM_REST 150
#define SERVO_PIN 14
#define MAX_OBJECTS 6


MotorDriver motorDriver;

int j = 0;

int rOn = 0;
int lOn = 0;
unsigned long curr_time = 0;
unsigned long out_time = 0;
unsigned long seek_time = 0;

/* This array will represent the positions of the board numbered 0-7
  7    0    1
    \  |  /
  6 -- * -- 2
    /  |  \
  5    4    3
*/
int positions[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int currentPos = 0;
int objectCount = 0;
int detectedColour;

int distance_readings = 0;
long objectDistance;

int tilesToObject;

enum states{
  waitForGo,
  findOne,
  goToOne,
  stopAndDetect,
  attack,
  turn,
  goBack,
  halt,
  justDistance //This is used for Module 2 Requirement 4 only
};

states state = waitForGo;

// Testing the colours and orders
// byte orderIn[6] = {1, 6, 4, 5, 3, 7};
// byte coloursIn[8] = {0, 1, 0, 1, 2, 3, 3, 2};
byte orderIn[6] = {-1, -1, -1, -1, -1, -1};
byte coloursIn[8] = {3, 3, 3, 3, 3, 3, 3, 3};

void setup() {
  Serial.begin(9600);
  motorDriver.begin();
  
  /* For IR Line Sensor */
  Wire.begin();  
  WriteByte(sensorAddr, 0x3, 0xFE);
  WriteByte(sensorAddr, 0x2, 0xFE);

  /* For Colour Sensor */  
  pinMode(COLOUR_PIN_S0_S1, OUTPUT);
  //pinMode(COLOUR_PIN_S1, OUTPUT);
  pinMode(COLOUR_PIN_S2, OUTPUT);
  pinMode(COLOUR_PIN_S3, OUTPUT);
  pinMode(COLOUR_OUT, INPUT);
  
  /* Setting frequency-scaling to 100% */
  digitalWrite(COLOUR_PIN_S0_S1,HIGH);
  
  pinMode(C_LED_PIN1, OUTPUT);
  pinMode(C_LED_PIN2, OUTPUT);
  pinMode(C_LED_PIN3, OUTPUT);

  /*Servo Arm */
  pinMode(SERVO_PIN, OUTPUT);

  setColour(NO_COLOUR);

  StartBluetooth();
  delay(3000); //Wait for Bluetooth to reconnect to the laptop
}

void loop() {
  // ------------- State Machine ----------------------
  switch(state)
  {
    // Wait for user to press start on the Bluetooth GUI --------------
    case waitForGo:
      WaitForBTGo();
      state = findOne;
    // ----------Find an object-------------------
    case findOne:
      /* If we haven't visited this one before, take multiple readings to be sure */
      if(distance_readings < DISTANCE_ATTEMPTS && positions[currentPos] == 0)
      {
        objectDistance = getDistance();
        Serial.print("object at distance: ");Serial.println(objectDistance);
        delay(50);
        if(distance_readings == 4)
        {
          motorDriver.Waddle(RIGHT);
        } else if (distance_readings == 9)
        {
          motorDriver.Waddle(RIGHT);
        } else if (distance_readings == 14)
        {
          motorDriver.Waddle(RIGHT);
        } else if (distance_readings == 19)
        {
          motorDriver.Waddle(RIGHT);
        } else if (distance_readings == 24)
        {
          motorDriver.Waddle(RIGHT);
        } else if (distance_readings == 29)
        {
          motorDriver.Waddle(LEFT);
          motorDriver.Waddle(LEFT);
          motorDriver.Waddle(LEFT);
          motorDriver.Waddle(LEFT);
          motorDriver.Waddle(LEFT);
        } else if (distance_readings == 34)
        {
          motorDriver.Waddle(LEFT);
        } else if (distance_readings == 39)
        {
          motorDriver.Waddle(LEFT);
        } else if (distance_readings == 44)
        {
          motorDriver.Waddle(LEFT);
          } else if (distance_readings == 49)
        {
          motorDriver.Waddle(LEFT);
          } else if (distance_readings == 54)
        {
          motorDriver.Waddle(LEFT);
          } else if (distance_readings == 59)
        {
          motorDriver.Waddle(RIGHT);
          motorDriver.Waddle(RIGHT);
          motorDriver.Waddle(RIGHT);
          motorDriver.Waddle(RIGHT);
          motorDriver.Waddle(RIGHT);
          delay(20);
        }
        
        if(objectDistance > 0 && objectDistance < MAX_OBJECT_DISTANCE) //found an object on this point
        {
          Serial.print("object at distance: ");Serial.println(objectDistance);
          state = goToOne; //next state
          orderIn[objectCount] = currentPos;
          objectCount++;
          positions[currentPos] = 1;
          lookForLine(motorDriver);
          SendBTGoing(currentPos);
          setColour(NO_COLOUR);
          seek_time = millis();
        } else
        {
          distance_readings++;
        }
      } else 
      {
        /* we've tried to find an object DISTANCE_ATTEMPTS times at this corner with no luck,
         * or we've visited this one already. Turn 45 degrees and try again!
         */
         motorDriver.turn45();
        // positions[currentPos] = 1; to skip spaces
         currentPos++;
         if(currentPos == 8) currentPos = 0;
         distance_readings = 0;
      }
      break;
    // ----------Go Towards the object until the distance is < STOP_DISTANCE cm -------------------
    case goToOne:
        tilesToObject = 0;
        while(objectDistance > STOP_DISTANCE)
        {
          //read line sensors & follow line
          readLineSensors(&rOn, &lOn);
          followLine(rOn, lOn, motorDriver);
          
          long last_distance = objectDistance;
          objectDistance = getDistance();
          delay(10);

          if(objectDistance < 0)
          {
            /* Someimes if it takes a reading while adjusting for the line it can miss the object,
             * since we're sure the object was there ignore this 'no object' reading by replacing it with
             * the most recent reading minus 1 centimeter.
             */
            objectDistance = last_distance - 1;
          }
          delay(10);
          Serial.print("object at distance: "); Serial.println(objectDistance);
        }
        
        //we've reached the stopping distance
        state = stopAndDetect; //next state
        SendBTStopped(currentPos);
        /* Keep track of how long it took us to get there so we can return */
        out_time = millis() - seek_time; 
      break;
    // ----------Stop and detect the objects colour -------------------
    case stopAndDetect:
      //stop
      motorDriver.stop(RMOTOR);
      motorDriver.stop(LMOTOR);
      delay(500);
      detectedColour = detectObjectColourAveraging();
      coloursIn[currentPos] = detectedColour;
      SendBTColour(currentPos, detectedColour);
      delay(500);
      curr_time = millis();
      if (detectedColour == RED)
        state = attack;
      else
        state = turn; //next state
      break;
    //-----------Attack the red object------------
    case attack:
      SwingArm();
      delay(100);
      state = turn;
      curr_time = millis();
      break;
    // ----------Turn around----------------------
    case turn:
      SendBTReturning(currentPos);
      motorDriver.turnAround();
      delay(100);
      state = goBack; //next state
      curr_time = millis();
      break;
    // ---------- return to the center-------------------
    case goBack:
      readLineSensors(&rOn, &lOn);
      followLine(rOn, lOn, motorDriver);
      /* Return to the middle base on a factor of the time it took us to get out there */
      if(millis() - curr_time > out_time * RETURN_FACTOR)
      {
        curr_time = millis();
        state = halt; //next state
      }
      //backToCenter(tilesToObject, motorDriver); 
      break;
    case halt:
      motorDriver.stop(RMOTOR);
      motorDriver.stop(LMOTOR);
      delay(100);
      //If we still have objects to find
      if(objectCount < MAX_OBJECTS)
      {
        /* update the position the robot is facing to account for the fact we returned 
         *  after an 180 degree turn.
         */
        if(currentPos < 4) {
          currentPos += 4;
        } else {
          currentPos -= 4;
        }
  
        distance_readings = 0;
        state = findOne; //look for the next one
      } else
      {
        /* We've found all! */
        setColour(GREEN);
        writeToEEPROM(orderIn, coloursIn);
        SendData();
      }
      break;
    case justDistance:
      delay(200);
      objectDistance = getDistance();
      Serial.print("object at distance: ");Serial.print(objectDistance);Serial.println(" cm");
      delay(100);
    default:
      break;
  }
  delay(50);
}

/* For Arm Servo */
// Swing the robot Arm ------------------------
void SwingArm()
{   
  int lenMicroSecondsOfPeriod = 20 * 1000; // 25 milliseconds (ms)
  int lenMicroSecondsOfPulse = 1 * 1000; // 1 ms is 0 degrees
  int firstPos = 0.65 * 1000; //first position
  int endPos = 1.7 * 1000;//first position
  int current = 0;
  int servoIncrement = 0.01 * 800;
  
  /* Servo works by sending a 25 ms pulse.  
     0.5 ms at the start of the pulse will turn the servo to the 0 degree position
     2.0 ms at the start of the pulse will turn the servo to the 90 degree position 
     3.0 ms at the start of the pulse will turn the servo to the 180 degree position 
  */
  for(current = endPos; current > firstPos; current -= servoIncrement){    
    //Turn voltage high to start the period and pulse
    digitalWrite(SERVO_PIN, HIGH);
    // Delay for the length of the pulse
    delayMicroseconds(current);
    // Turn the voltage low for the remainder of the pulse
    digitalWrite(SERVO_PIN, LOW);
    // Delay for the remainder of the period
    delayMicroseconds(lenMicroSecondsOfPeriod - current); 
  }
  for(current = firstPos; current < endPos; current += servoIncrement){
    //Turn voltage high to start the period and pulse
    digitalWrite(SERVO_PIN, HIGH);
    // Delay for the length of the pulse
    delayMicroseconds(current);
    // Turn the voltage low for the remainder of the pulse
    digitalWrite(SERVO_PIN, LOW);
    // Delay for the remainder of the period
    delayMicroseconds(lenMicroSecondsOfPeriod - current); 
  }
}
