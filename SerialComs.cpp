#include "SerialComs.h"

int bluetoothTx = 16;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 15;  // RX-I pin of bluetooth mate, Arduino D3

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

void SendBTGoing(int pos)
{     
  if(bluetooth.available())
  {
    bluetooth.print('G');
    bluetooth.print(pos);
    bluetooth.print('E');
  }
}
void SendBTStopped(int pos)
{     
  if(bluetooth.available())
  {
    bluetooth.print('S');
    bluetooth.print(pos);
    bluetooth.print('E');
  }
}
void SendBTColour(int pos, int colour)
{     
  if(bluetooth.available())
  {
    bluetooth.print('C');
    bluetooth.print(pos);
    bluetooth.print(colour);
    bluetooth.print('E');
  }
}
void SendBTReturning(int pos)
{
  if(bluetooth.available())
  {
    bluetooth.print('R');
    bluetooth.print(pos);
    bluetooth.print('E');
  }
}

void WaitForBTGo()
{
  while(true)
  {
    if(bluetooth.read() == 'B')
    {
      break;
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
