#include "MotorDriver.h"
#include "LineSensing.h"
#include "ColourSensing.h"
#include "DistanceSensing.h"

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

int distance_readings;
long objectDistance;

enum states{
  findOne,
  goToOne,
  stopAndDetect,
  turn,
  goBack,
  halt,
  justDistance
};

states state = findOne;

void setup() {
  Serial.begin(9600);
  motorDriver.begin();
  
  /* For IR Line Sensor */
  Wire.begin();  
  WriteByte(sensorAddr, 0x3, 0xFE);
  WriteByte(sensorAddr, 0x2, 0xFE);

  /* For Ultra Sonic Distance Sensor */ 

  /* For Colour Sensor */  
  pinMode(COLOUR_PIN_S0_S1, OUTPUT);
  //pinMode(COLOUR_PIN_S1, OUTPUT);
  pinMode(COLOUR_PIN_S2, OUTPUT);
  pinMode(COLOUR_PIN_S3, OUTPUT);
  pinMode(COLOUR_OUT, INPUT);
  
  // Setting frequency-scaling to 100%
  digitalWrite(COLOUR_PIN_S0_S1,HIGH);
  //digitalWrite(COLOUR_PIN_S1,HIGH);
  
  pinMode(C_LED_PIN1, OUTPUT);
  pinMode(C_LED_PIN2, OUTPUT);
  pinMode(C_LED_PIN3, OUTPUT);

  distance_readings = 0;
}

void loop() {
  // ------------- State Machine ----------------------
  /*
  for(int i = 0; i < 8; i++)
  {
    Serial.print(positions[i]);
  }
  */
  //Serial.print("curpos: ");Serial.println(currentPos);
  switch(state)
  { 
    // ----------Find an object-------------------
    case findOne:
      //setColour(GREEN);
      if(distance_readings < DISTANCE_ATTEMPTS)
      {
        objectDistance = getDistance();
        Serial.print("object at distance: ");Serial.println(objectDistance);
        delay(50);
        if(objectDistance > 0 && objectDistance < MAX_OBJECT_DISTANCE) //found an object on this point
        {
          Serial.print("object at distance: ");Serial.println(objectDistance);
          if(positions[currentPos] == 0)
          {
            state = goToOne;
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
          //Serial.println("No object detected!");
          distance_readings++;
        }
      } else 
      {
        /* we've tried to find an object five times at this corner with no luck,
         * turn 45 degrees and try again!
         */
         motorDriver.turn45();
         currentPos++;
         if(currentPos == 8) currentPos = 0;
         distance_readings = 0;
      }
      break;
    // ----------Go Towards the object until the distance is < 5 cm -------------------
    case goToOne:
        while(objectDistance > STOP_DISTANCE)
        {
          //read line sensors & follow line
          readLineSensors(&rOn, &lOn);
          //drive base on line sensors
          //Serial.print("rOn: ");Serial.print(rOn);Serial.print("lOn: ");Serial.println(lOn);
          followLine(rOn, lOn, motorDriver);

          //distance_readings = 0;
          long last_distance = objectDistance;
          //do {
            objectDistance = getDistance();
          //  if(objectDistance < last_distance - 15) objectDistance = -1; //temp
          //  distance_readings++;
            delay(10);
          //} while (objectDistance < 0 && distance_readings < DISTANCE_ATTEMPTS);

          if(objectDistance < 0)
          {
            //Something went rather wrong :(
            //setColour(GREEN);
            objectDistance = last_distance - 1;
            //distance_readings = 0;
          }
          delay(10);
          Serial.print("object at distance: "); Serial.println(objectDistance);
        }
        //we've reached the stopping distance
        state = stopAndDetect;
        out_time = millis() - seek_time;
      break;
    // ----------Stop and detect the objects colour -------------------
    case stopAndDetect:
    Serial.println("Sensing colour now");
      //setColour(RED);
      motorDriver.stop(RMOTOR);
      motorDriver.stop(LMOTOR);
      delay(500);
      detectObjectColourAveraging();
      delay(500);
      curr_time = millis();
      state = turn;
      break;
    // ----------Turn around-------------------
    case turn:
      motorDriver.turnAround();
      delay(100);
      state = goBack;
      curr_time = millis();
      break;
       // ---------- return to the center-------------------
    case goBack:
      readLineSensors(&rOn, &lOn);
      followLine(rOn, lOn, motorDriver); //change to backwards?
      //Serial.print("rOn: ");Serial.print(rOn);Serial.print("lOn: ");Serial.println(lOn);
      if(millis() - curr_time > out_time * RETURN_FACTOR)
      {
        curr_time = millis();
        state = halt;
      }
      break;
    case halt:
      motorDriver.stop(RMOTOR);
      motorDriver.stop(LMOTOR);
      delay(100);
      //If we still have objects to find
      if(objectCount < 3)
      {
        if(currentPos < 4) 
        {
          currentPos += 4;
        } else
        {
          currentPos -= 4;
        }
  
        distance_readings = 0;
        
        state = findOne;
      } else
      {
        setColour(GREEN);
      }
      break;
    case justDistance:
      objectDistance = getDistance();
      Serial.print("object at distance: ");Serial.print(objectDistance);Serial.println(" cm");
      delay(500);
     // detectObjectColourAveraging();
      delay(500);
    default:
      break;
  }
  delay(50);
}

