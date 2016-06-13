#include "WheelEncoders.h"

int countWhite;
int countBlack; 
float totalDistance;
int numberOfTiles;

/*
 * Purpose: Compares the number of tiles of both wheels, if they are unequal,
 *          return the number 1 (RIGHT) or 0 (LEFT) of the motor that needs 
 *          to spin more to follow a straight line. 
 * Input:   NONE.
 * Output:  tells which wheel to turn.
 * Obs:     The motor control is done in MotorDriver.cpp
 */
void goForward(MotorDriver motorDriver){
  int tilesRight = countTiles(RIGHT_SENSOR);
  int tilesLeft = countTiles(LEFT_SENSOR);

  while (tilesRight != tilesLeft){
    if (tilesRight > tilesLeft){
      motorDriver.speed(LMOTOR, 100);
    }
    else{
      motorDriver.speed(RMOTOR, 100);
    }
  }
  motorDriver.speed(RMOTOR, 100);  
  motorDriver.speed(LMOTOR, 100);
  delay(50);
}

/*
 * Purpose: counts the number of tiles read by sensor.
 * 
 * Inputs:  sensor to read from.
 * Oututs:  NONE.
 */
int countTiles(int analogPin) {
  int currentState = blackOrWhite(analogPin);
  int previousState;
  
  if (currentState == BLACK && previousState == WHITE){
    numberOfTiles++;
    previousState = currentState;
  }
  else if (currentState == WHITE && previousState == BLACK){
    numberOfTiles++;
    previousState == currentState;
  }

  return numberOfTiles;
}

/*
 * Purpose:  tells if the sensor is detecting a back or white tile.
 */
int blackOrWhite(int analogPin){
  int sensorValue = analogRead(analogPin);
  if (sensorValue < THRESHOLD){
    return 1;                       // Sensor detects a white tile
  }
  else{
    return 0;                       // Sensor detects a black tile
  }
  //Serial.println(sensorValue)
}

/*
 * Purpose:  calculaes and displays the distance traveled by the robot.
 * Return:   a float that is the total distance.
 */
float getTotalDistance(){
   /*
   * Formula for distance per "tile"
   * 2piR/numberOfTiles
   */
  int tiles = countBlack+countWhite;
  totalDistance = ((6.28 * WHEEL_RADIUS * tiles)/ TOTAL_TILES);
 
  return totalDistance;
}
// End of program
