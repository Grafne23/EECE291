#include "WheelEncoders.h"
#include "Arduino.h"

int tiles = 0;
int previousState;
float totalDistance;
 
float getTotalDistance(){
   /*
   * Formula for distance per "tile"
   * 2piR/numberOfTiles
   */
  totalDistance = ((6.28 * WHEEL_RADIUS * tiles)/ TOTAL_TILES);
 
  return totalDistance;
}

void countTiles() {
  int currentState = blackOrWhite(RIGHT_WHEEL);
  //if (analogRead(RIGHT_WHEEL) 800)
    
  
  if (currentState == BLACK && previousState == WHITE){

    tiles++;
    previousState = currentState;
  }
  if (currentState == WHITE && previousState == BLACK){

    tiles++;
    previousState = currentState;
  }
  Serial.print("Tiles: "); Serial.println(tiles); 
} 

/*if(analogRead(RIGHT_WHEEL) == THRESHOLD )
{
tiles++;
}
Serial.print("Tiles: "); Serial.println(tiles); 
}*/
/*
 * This function tells if the sensor is detecting a back or white tile.
 */

int blackOrWhite(int analogPin){
  Serial.print("Analog read:  "); Serial.println(analogRead(analogPin));
  int sensorValue = analogRead(analogPin);
  if (sensorValue < THRESHOLD){
    return 1;                       // Sensor detects a white tile
  }
  else{
    return 0;                       // Sensor detects a black tile
  }
  //Serial.println(sensorValue)
}

// End of program
