// OSEPP IR Proximity Sensor Example Sketch
// by OSEPP 
// This sketch demonstrates interactions with the IR Proximity Sensor #include 
// Possible sensor addresses (suffix correspond to DIP switch positions) 

#include <Wire.h>

#define SENSOR_ADDR_OFF_OFF (0x26) 

#define SENSOR_ADDR_OFF_ON (0x22) 

#define SENSOR_ADDR_ON_OFF (0x24) 

#define SENSOR_ADDR_ON_ON (0x20) 

// Set the sensor address here 
const uint8_t sensorAddr = SENSOR_ADDR_ON_ON; 

// One-time setup 
void setup() { 
  
  // Start the serial port for output 
  Serial.begin(9600);                           // Join the I2C bus as master 
  Wire.begin();                                 // Turn on the sensor by configuring pin 1 of the GPIO expander to be an 
                                                // output pin; the default output value is already HI so there's no need 
                                                // to change it 
  WriteByte(sensorAddr, 0x3, 0xFE);
} 
  
// Main program loop 
  void loop() { 
    uint8_t val;                                // Get the value from the sensor 
    
    if (ReadByte(sensorAddr, 0x0, &val) == 0) 
    {                                           // The second LSB indicates if something was not detected, i.e., 
                                                // LO = object detected, HI = nothing detected 
    if (val & 0x2) 
      { 
      Serial.println("Nothing detected"); 
      } 
    else 
      { 
      Serial.println("Object detected"); 
      } 
    } 
   
  else 
    { 
      Serial.println("Failed to read from sensor");   
    } 
   // Run again in 1 s (1000 ms) 
  delay(1000); 
    
  float volts = val*0.0048828125; 
  float distance = 65*pow(volts, -1.10); 
    
  Serial.println(distance);  

}
        
 // Read a byte on the i2c interface 
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
      // Read nothing back 
      return -1;
      } 
    return 0; 
    } 
           
// Write a byte on the i2c interface 
  void WriteByte(uint8_t addr, uint8_t reg, byte data) { 
    // Begin the write sequence 
    Wire.beginTransmission(addr);               // First byte is to set the register pointer 
    Wire.write(reg);                            // Write the data byte 
    Wire.write(data);                           // End the write sequence; bytes are actually transmitted now 
    
    Wire.endTransmission(); 
   } 

