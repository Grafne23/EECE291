#include "MotorDriver.h"
#include "Arduino.h"

#define RIGHT_SENSOR  A1
#define LEFT_SENSOR   A2

#define THRESHOLD       820               // Value used at blackOrWhite function, above 820 = black, 
#define WHEEL_RADIUS    3.4               //below 820 = white
#define TOTAL_TILES     12
#define ONE_TILE        1.78
#define TILES_DIFFERENCE 3

#define BLACK 0
#define WHITE 1

#define RIGHT 0
#define LEFT  1
  
void backToCenter(int howFar, MotorDriver motorDriver);
int countTiles(int analogPin, int tilesVariable);
int blackOrWhite(int analogPin);
void resetTiles(int globalTiles);
int cmToTiles(int distance);
