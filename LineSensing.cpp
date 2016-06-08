#include "MotorDriver.h"
#include "LineSensing.h"
#include <Wire.h>

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

// Controls motor based on sensor values --------------------
void followLine(int rOn, int lOn, MotorDriver motorDriver)
{
  if (rOn == 1 && lOn == 1)     // move forward
    {
    motorDriver.speed(RMOTOR, 100);  
    motorDriver.speed(LMOTOR, 100);
    delay(500);
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

void followLineBackwards(int rOn, int lOn, MotorDriver motorDriver)
{
  if (rOn == 1 && lOn == 1)     // move backwards
    {
    motorDriver.speed(RMOTOR, -100);  
    motorDriver.speed(LMOTOR, -100);
    delay(1000);
    }
  
  if (rOn == 0 && lOn == 1)     // veer left
    {
    motorDriver.speed(RMOTOR, -100);  
    motorDriver.speed(LMOTOR, -50);
    delay(70);
    }
  
  if (rOn == 1 && lOn == 0)     // veer right
    {
    motorDriver.speed(RMOTOR, -50);
    motorDriver.speed(LMOTOR, -100);
    delay(70);
    }
  
  if (rOn == 0 && lOn == 0)     // spin on the spot
  {
    motorDriver.speed(RMOTOR, 0);
    motorDriver.speed(LMOTOR, 0);
    delay(100);
  }

}

// Reading values from line sensor -----------------------------
void readLineSensor(int sensorByte, int *rOn, int *lOn){
  uint8_t sensorValue;
  if (ReadByte(sensorAddr, sensorByte, &sensorValue) == 0)
  {
    if(sensorValue > LINETHRESHOLD)
    {
      if(sensorByte == RSENSOR) 
      {
        *rOn = 1;
      }
      else 
      {
        *lOn = 1;
      }
    } 
    else
    {
      if(sensorByte == RSENSOR) 
      {
        *rOn = 0;
      }
      else 
      {
        *lOn = 0;
      }
    }
  } 
  else 
  { 
    Serial.println("Failed to read from  a sensor");   
  }
}

// Read values from all line sensors -----------------------------
void readLineSensors(int *rOn, int *lOn){
  uint8_t sensorValue;
  int centerVal = analogRead(A0);
  //Serial.print("centerVal: ");Serial.println(centerVal);
  //delay(50);
  
  //if center sensor is on then we're okay
  //if(centerVal > CENTERTHRESHOLD) //(center is on)
  if(false)
  {
    *rOn = 1;
    *lOn = 1;
    return;
  }
  else
  {
    //otherwise read the two sides
    readLineSensor(RSENSOR, rOn, lOn);
    readLineSensor(LSENSOR, rOn, lOn); 
    //3 options center on with either side off or all off.
    return; // ? - might just work.
  }
}


