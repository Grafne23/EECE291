#include "MotorDriver.h"
#include "LineSensing.h"
#include "ColourSensing.h"
#include "DistanceSensing.h"
#include "SerialComs.h"

#include <Wire.h>

#define STRAIGHT1_DONE 10000
#define DISTANCE_ATTEMPTS 10
#define STOP_DISTANCE 2
#define MAX_OBJECT_DISTANCE 80
#define RETURN_FACTOR 0.90

MotorDriver motorDriver;

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
int positions[8] = {0};
int currentPos = 0;
int objectCount = 0;

int distance_readings = 0;
long objectDistance;

enum states{
  findOne,
  goToOne,
  stopAndDetect,
  turn,
  goBack,
  halt,
  justDistance //This is used for Module 2 Requirement 4 only
};

states state = justDistance;

// Testing the colours and orders
byte orderIn[6] = {1, 6, 4, 5, 3, 7};
byte coloursIn[8] = {0, 1, 0, 1, 2, 3, 3, 2};

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
  
  // Setting frequency-scaling to 100%
  digitalWrite(COLOUR_PIN_S0_S1,HIGH);
  
  pinMode(C_LED_PIN1, OUTPUT);
  pinMode(C_LED_PIN2, OUTPUT);
  pinMode(C_LED_PIN3, OUTPUT);
}

void loop() {
  // ------------- State Machine ----------------------
  switch(state)
  { 
    // ----------Find an object-------------------
    case findOne:
      if(distance_readings < DISTANCE_ATTEMPTS) //Take multiple readings to be sure
      {
        objectDistance = getDistance();
        Serial.print("object at distance: ");Serial.println(objectDistance);
        delay(50);
        if(objectDistance > 0 && objectDistance < MAX_OBJECT_DISTANCE) //found an object on this point
        {
          Serial.print("object at distance: ");Serial.println(objectDistance);
          if(positions[currentPos] == 0) //If we haven't visited this one before
          {
            state = goToOne; //next state
            objectCount++;
            seek_time = millis();
            setColour(NO_COLOUR);
            positions[currentPos] = 1;
            lookForLine(motorDriver);
          } else
          {
            distance_readings++;
          }
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
         currentPos++;
         if(currentPos == 8) currentPos = 0;
         distance_readings = 0;
      }
      break;
    // ----------Go Towards the object until the distance is < STOP_DISTANCE cm -------------------
    case goToOne:
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
        /* Keep track of how long it took us to get there so we can return */
        out_time = millis() - seek_time; 
      break;
    // ----------Stop and detect the objects colour -------------------
    case stopAndDetect:
      //stop
      motorDriver.stop(RMOTOR);
      motorDriver.stop(LMOTOR);
      delay(500);
      detectObjectColourAveraging();
      delay(500);
      curr_time = millis();
      state = turn; //next state
      break;
    // ----------Turn around-------------------
    case turn:
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
      break;
    case halt:
      motorDriver.stop(RMOTOR);
      motorDriver.stop(LMOTOR);
      delay(100);
      //If we still have objects to find
      if(objectCount < 3)
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
        /* We've found three! */
        setColour(GREEN);
      }
      break;
    case justDistance:
      objectDistance = getDistance();
      Serial.print("object at distance: ");Serial.print(objectDistance);Serial.println(" cm");
      delay(500);
      writeToEEPROM(orderIn, coloursIn);
      SendData();
     // detectObjectColourAveraging();
     // delay(500);
    default:
      break;
  }
  delay(50);
}

