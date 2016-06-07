#include "MotorDriver.h"
#include "LineSensing.h"
#include "ColourSensing.h"
#include "DistanceSensing.h"

#include <Wire.h>

#define STRAIGHT1_DONE 10000
#define DISTANCE_ATTEMPTS 5
#define STOP_DISTANCE 2


MotorDriver motorDriver;

int rOn = 0;
int lOn = 0;
unsigned long curr_time = 0;
unsigned long out_time = 0;

int distance_readings;
long objectDistance;

enum states{
  findOne,
  goToOne,
  stopAndDetect,
  turn,
  goBack,
  halt
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
  
  // Setting frequency-scaling to 20%
  digitalWrite(COLOUR_PIN_S0_S1,HIGH);
  //digitalWrite(COLOUR_PIN_S1,HIGH);
  
  pinMode(C_LED_PIN1, OUTPUT);
  pinMode(C_LED_PIN2, OUTPUT);
  pinMode(C_LED_PIN3, OUTPUT);

  distance_readings = 0;
}

void loop() {
  // ------------- State Machine ----------------------
  switch(state)
  { 
    // ----------Find an object-------------------
    case findOne:
      setColour(GREEN);
      if(distance_readings < DISTANCE_ATTEMPTS)
      {
        objectDistance = getDistance();
        delay(10);
        if(objectDistance > 0 && objectDistance < 90) //found an object on this corner
        {
          Serial.print("object at distance: ");Serial.println(objectDistance);
          state = goToOne;
          setColour(NO_COLOUR);
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
          Serial.print("rOn: ");Serial.print(rOn);Serial.print("lOn: ");Serial.println(lOn);
          followLine(rOn, lOn, motorDriver);

          distance_readings = 0;
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
            distance_readings = 0;
          }
          delay(10);
          Serial.print("object at distance: "); Serial.println(objectDistance);
        }
        //we've reached the stopping distance
        state = stopAndDetect;
        out_time = millis();
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
      state = goBack;
      delay(100);
      break;
       // ---------- return to the center-------------------
    case goBack:
      readLineSensors(&rOn, &lOn);
      followLine(rOn, lOn, motorDriver); //change to backwards?
      //Serial.print("rOn: ");Serial.print(rOn);Serial.print("lOn: ");Serial.println(lOn);
      if(millis() - curr_time > out_time * 1.05)
      {
        curr_time = millis();
        state = halt;
      }
      break;
    case halt:
      motorDriver.stop(RMOTOR);
      motorDriver.stop(LMOTOR);
      delay(100);
      break;
    default:
      break;
  }
  delay(50);
}

