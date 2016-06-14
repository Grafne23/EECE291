#include <EEPROM.h>
#include <SoftwareSerial.h>
#include "Arduino.h"

void StartBluetooth();
void SendBTData();
void SendData();
void writeToEEPROM(byte orderIn[], byte coloursIn[]);
