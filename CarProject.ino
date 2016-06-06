#include "MotorDriver.h"
#include "LineSensing.h"
#include <Wire.h>

#define RMOTOR 0                             //right motor pin
#define LMOTOR 1                             //left motor pin

#define STRAIGHT1_DONE 10000

MotorDriver motorDriver;

int rOn = 0;
int lOn = 0;
unsigned long curr_time = 0;

enum states{
  straight1,
};

states state = straight1;

void setup() {
  Serial.begin(9600);
  motorDriver.begin();
  Wire.begin();  
  
  WriteByte(sensorAddr, 0x3, 0xFE);
  WriteByte(sensorAddr, 0x2, 0xFE);
}

void loop() {
  switch(state)
  { 
    case straight1:
        //read line sensors
        readLineSensors(&rOn, &lOn);
        //drive base on line sensors
        Serial.print("rOn: ");Serial.print(rOn);Serial.print("lOn: ");Serial.println(lOn);
        followLine(rOn, lOn, motorDriver);
        
        // Next state logic:
        // advance to turn state if sufficient time has passed
        if(millis() - curr_time > STRAIGHT1_DONE)
        {
          curr_time = millis();
          delay(10000);
        }
        break;
    default:
      break;
  }
}
