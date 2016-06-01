#include "MotorDriver.h"
#include <Wire.h>

#define RMOTOR 0
#define LMOTOR 1
#define RSENSOR 0x1
#define LSENSOR 0x0

#define SENSOR_ADDR_OFF_OFF (0x4B)  
#define SENSOR_ADDR_OFF_ON (0x49) 
#define SENSOR_ADDR_ON_OFF (0x4A) 
#define SENSOR_ADDR_ON_ON (0x48) 
// Set the sensor address here 
const uint8_t sensorAddr = SENSOR_ADDR_ON_OFF; 

#define LINETHRESHOLD 235
#define STRAIGHT_DONE 37000
#define WAIT_TIME 2000
#define DEGREE_TIME 600

MotorDriver motorDriver;

int rOn = 0;
int lOn = 0;
uint8_t rSensorVal;
uint8_t lSensorVal;
unsigned long curr_time = 0;

enum states{
  straight,
  turn,
  back,
  wait,
  turnMore,
  halt
};

states state = straight;

void setup()
{
  Serial.begin(9600);
  motorDriver.begin();
  Wire.begin();  
  
  WriteByte(sensorAddr, 0x3, 0xFE);
  WriteByte(sensorAddr, 0x2, 0xFE);
}

void loop()
{ 
 // ------------- State Machine ----------------------
 switch(state)
 {
 // ----------Go Forward for 2 Meters-------------------
 case straight:
    //read line sensors
    readLineSensor(RSENSOR, &rSensorVal);
    readLineSensor(LSENSOR, &lSensorVal);
    //drive base on line sensors
    followLine();
    
    // Next state logic:
    // advance to turn state if sufficient time has passed
    if(millis() - curr_time > STRAIGHT_DONE)
    {
      curr_time = millis();
      delay(200);
      state = turn;
    }
    break;
 // ----------Turn 180 degrees-------------------
 case turn:
    TurnAround();
    // Next state logic:
    // always advance to back state
    state = back;
    delay(200);
    curr_time = millis();
    break;
 // ----------Drive Back 1 Meter-------------------
 case back:
    //read line sensors
    readLineSensor(RSENSOR, &rSensorVal);
    readLineSensor(LSENSOR, &lSensorVal);
    //drive base on line sensors
    followLine();
    
    // Next state logic:
    // advance to wait state if sufficient time has passed
    if(millis() - curr_time > (STRAIGHT_DONE * 0.50))
    {
      state = wait;
      curr_time = millis();
    }
    break;
 // ----------Wait 2 Seconds-------------------
 case wait:
    // be stopped in wait state
    motorDriver.stop(RMOTOR);  
    motorDriver.stop(LMOTOR);
    // Next state logic:
    // advance to turnMore state after wait_time
    if(millis() - curr_time > WAIT_TIME)
    {
      state = turnMore;
    }
    break;
     // -----Turn 180 degrees in increments of 45--------
 case turnMore:
    //Turn 45, Wait a second, repeat 4 times
    for(int i = 0; i < 4; i ++)
    {
      Turn45();
      delay(1000);
    }
    // Next state logic:
    // always advance to halt state
    state = halt;
    break;
 // ----------Wait Forever-------------------
 case halt:
    // Next state logic:
    // stay in halt state
    state = halt;
    break;
 default:
    Serial.println("Invalid state!");
    break;
 }
  
}

// Read a byte on the i2c interface --------------------------
int ReadByte(uint8_t addr, uint8_t reg, uint8_t *data) { 
  // Do an i2c write to set the register that we want to read from 
  Wire.beginTransmission(addr); 
  Wire.write(reg); 
  Wire.endTransmission(); 
  // Read a byte from the device 
  Wire.requestFrom(addr, (uint8_t) 1); 
    
  if (Wire.available()) 
  {
    *data = Wire.read(); 
  } 
  else 
  { 
    return -1; // Read nothing back 
  } 
  
  return 0; 
} 
           
// Write a byte on the i2c interface ------------------------ 
void WriteByte(uint8_t addr, uint8_t reg, byte data) { 
  // Begin the write sequence 
  Wire.beginTransmission(addr);   // First byte is to set the register pointer 
  Wire.write(reg);                // Write the data byte 
  Wire.write(data);               // End the write sequence; bytes are actually transmitted now 
    
  Wire.endTransmission(); 
} 

// Turn the robot around ---------------------
void TurnAround()
{
  motorDriver.speed(RMOTOR, 90);  
  motorDriver.speed(LMOTOR, -90);
  delay(300);
    
  while(true)
  {
    // spin right, reading the line sensors
    motorDriver.speed(RMOTOR, 90);  
    motorDriver.speed(LMOTOR, -90);
    readLineSensor(RSENSOR, &rSensorVal);
    readLineSensor(LSENSOR, &lSensorVal);
    delay(10);
    // If it doesnt find the line on spins again try this.
   
    if(rOn == 1)
    {
      //we found the line, veer right
       motorDriver.speed(RMOTOR, 100);  
       motorDriver.speed(LMOTOR, 80);
       break;
    }
    
    /*
    //once we hit the line stop and return;
    if (rOn == 1 && lOn == 1) 
    {
        motorDriver.stop(RMOTOR);  
        motorDriver.stop(LMOTOR);
        break;
    }
    */
  }
}

// Turn the robot 45degrees ------------------------
void Turn45()
{
  curr_time = millis();
  //spin right for DEGREE_curr_time
  while(millis() - curr_time < DEGREE_TIME)
  {
      motorDriver.speed(RMOTOR, 100);  
      motorDriver.speed(LMOTOR, -100);
  }
  
  motorDriver.stop(RMOTOR);  
  motorDriver.stop(LMOTOR);
}

// Controls motor based on sensor values --------------------
void followLine()
{
  if (rOn == 1 && lOn == 1)     // move forward
    {
    motorDriver.speed(RMOTOR, 100);  
    motorDriver.speed(LMOTOR, 100);
    delay(1000);
    }
  
  if (rOn == 0 && lOn == 1)     // veer left
    {
    motorDriver.speed(RMOTOR, 85);  
    motorDriver.speed(LMOTOR, 100);
    delay(70);
    }
  
  if (rOn == 1 && lOn == 0)     // veer right
    {
    motorDriver.speed(RMOTOR, 100);  
    motorDriver.speed(LMOTOR, 85);
    delay(70);
    }
  
  if (rOn == 0 && lOn == 0)     // spin on the spot
  {
    motorDriver.speed(RMOTOR, 100); 
    motorDriver.speed(LMOTOR, -100);
    delay(100);
  }

}

// Reading values from line sensor -----------------------------
void readLineSensor(int sensorByte, uint8_t *sensorValue){
 if (ReadByte(sensorAddr, sensorByte, sensorValue) == 0)
   {
      if(*sensorValue > LINETHRESHOLD)
      {
        if(sensorByte == RSENSOR) 
        {
          rOn = 1;
        }
        else 
        {
          lOn = 1;
        }
      } 
      else
      {
        if(sensorByte == RSENSOR) 
        {
          rOn = 0;
        }
        else 
        {
          lOn = 0;
        }
      }
   } 
  else 
    { 
      Serial.println("Failed to read from  a sensor");   
    }

}



