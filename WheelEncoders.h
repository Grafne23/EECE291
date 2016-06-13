#include "MotorDriver.h"
#include "Arduino.h"

#define RIGHT_SENSOR A1
#define LEFT_SENSOR A2

#define THRESHOLD 820               // Value used at blackOrWhite function, above 820 = black, below 820 = white
#define WHEEL_RADIUS 3.4
#define TOTAL_TILES 12

#define BLACK 0
#define WHITE 1

#define RIGHT 0
#define LEFT  1
  
float getTotalDistance();
int blackOrWhite(int analogPin);
int countTiles(int analogPin);
void goForward(MotorDriver motorDriver);
