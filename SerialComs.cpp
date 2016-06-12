#include "SerialComs.h"

byte order[6] = {0};
byte colours[8] = {3};

byte orderAdresses[6] = {0, 1, 2, 3, 4, 5};
byte colourAddresses[8] = {6, 7, 8, 9, 10, 11, 12, 13};

// read the serial port
int GetFromSerial()
{
  while (Serial.available()<=0) {
  }
  return Serial.read();
}

//write the given values to the EEPROM
void writeToEEPROM(byte orderIn[], byte coloursIn[])
{
  for(int i = 0; i < 6; i++)
  {
    EEPROM.write(orderAdresses[i], orderIn[i]);
  }
  for(int i = 6; i < 14; i++)
  {
    EEPROM.write(colourAddresses[i - 6], coloursIn[i - 6]);
  }
}

//read the order and colour values from the EEPROM
void readFromEEPROM()
{
  for(int i = 0; i < 6; i++)
  {
    order[i] = EEPROM.read(orderAdresses[i]);
  }
  for(int i = 6; i < 14; i++)
  {
    colours[i - 6] = EEPROM.read(colourAddresses[i - 6]);
  }
}

//Send all the data to the serial Port
void SendData()
{      
  readFromEEPROM();
  if(Serial.available()!=0)
  {
    // call the returned value from GetFromSerial() function
    switch(GetFromSerial())
    {
    case 'Q':
      //Serial.flush();
      delay(10);
      Serial.write('A');
      Serial.write(order[0]);
      Serial.write('B');
      Serial.write(order[1]);
      Serial.write('C');
      Serial.write(order[2]);
      Serial.write('D');
      Serial.write(order[3]);
      Serial.write('E');
      Serial.write(order[4]);
      Serial.write('F');
      Serial.write(order[5]);
      Serial.write('G');
      Serial.write(colours[0]);
      Serial.write('H');
      Serial.write(colours[1]);
      Serial.write('I');
      Serial.write(colours[2]);
      Serial.write('J');
      Serial.write(colours[3]);
      Serial.write('K');
      Serial.write(colours[4]);
      Serial.write('L');
      Serial.write(colours[5]);
      Serial.write('M');
      Serial.write(colours[6]);
      Serial.write('N');
      Serial.write(colours[7]);
      Serial.flush();
      break;
    }
  } 
}
