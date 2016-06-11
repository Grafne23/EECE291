#define RIGHT_WHEEL A1
#define LEFT_WHEEL A2

#define THRESHOLD 820               // Value used at blackOrWhite function, above 820 = black, below 820 = white
#define WHEEL_RADIUS 3.4
#define TOTAL_TILES 16

#define BLACK 0
#define WHITE 1
  
float getTotalDistance();
int blackOrWhite(int analogPin);
void countTiles();

