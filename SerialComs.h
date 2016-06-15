#include <EEPROM.h>
#include <SoftwareSerial.h>
#include "Arduino.h"

void StartBluetooth();
void SendBTData();
void SendData();
void writeToEEPROM(byte orderIn[], byte coloursIn[]);
void SendBTGoing(int pos);
void SendBTStopped(int pos);
void SendBTColour(int pos, int colour);
void SendBTReturning(int pos);
void WaitForBTGo();
