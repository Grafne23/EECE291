#include "SerialComs.h"

int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

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

void StartBluetooth()
{
  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$$$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600
}

void SendBTData()
{      
  Serial.println("reading");
  readFromEEPROM();
  byte orderIn2[6] = {1, 6, 4, 5, 3, 7};
  byte coloursIn2[8] = {0, 1, 0, 1, 2, 3, 3, 2};
  if(bluetooth.available())
  {
    switch(bluetooth.read())
    {
      case 'Q':
    //  Serial.println("Read a Q!");
      delay(10);
      bluetooth.print(orderIn2[0]);
      bluetooth.print(orderIn2[1]);
      bluetooth.print(orderIn2[2]);
      bluetooth.print(orderIn2[3]);
      bluetooth.print(orderIn2[4]);
      bluetooth.print(orderIn2[5]);
      bluetooth.print(coloursIn2[0]);
      bluetooth.print(coloursIn2[1]);
      bluetooth.print(coloursIn2[2]);
      bluetooth.print(coloursIn2[3]);
      bluetooth.print(coloursIn2[4]);
      bluetooth.print(coloursIn2[5]);
      bluetooth.print(coloursIn2[6]);
      bluetooth.print(coloursIn2[7]);
    }
  }
}

//Send all the data to the serial Port
void SendBTDataOld()
{      
  Serial.println("reading");
  readFromEEPROM();
  if(bluetooth.available())
  {
    switch(bluetooth.read())
    {
      case 'Q':
    //  Serial.println("Read a Q!");
      delay(10);
      bluetooth.println('A');
      bluetooth.println(order[0]);
      bluetooth.println('B');
      bluetooth.println(order[1]);
      bluetooth.println('C');
      bluetooth.println(order[2]);
      bluetooth.println('D');
      bluetooth.println(order[3]);
      bluetooth.println('E');
      bluetooth.println(order[4]);
      bluetooth.println('F');
      bluetooth.println(order[5]);
      bluetooth.println('G');
      bluetooth.println(colours[0]);
      bluetooth.println('H');
      bluetooth.println(colours[1]);
      bluetooth.println('I');
      bluetooth.println(colours[2]);
      bluetooth.println('J');
      bluetooth.println(colours[3]);
      bluetooth.println('K');
      bluetooth.println(colours[4]);
      bluetooth.println('L');
      bluetooth.println(colours[5]);
      bluetooth.println('M');
      bluetooth.println(colours[6]);
      bluetooth.println('N');
      bluetooth.println(colours[7]);
    }
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
