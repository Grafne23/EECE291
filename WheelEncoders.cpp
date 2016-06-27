#include "WheelEncoders.h"

int tilesRight = 0;
int tilesLeft = 0;

/*
 * Purpose: Compares the number of tiles of both wheels, if they are unequal,
 *          return the number 1 (RIGHT) or 0 (LEFT) of the motor that needs 
 *          to spin more to follow a straight line. 
 * Input:   tiles to the object, motorDriver.
 * Output:  tells which wheel to turn.
 */
void backToCenter(int howFar, MotorDriver motorDriver){
  countTiles(RIGHT_SENSOR, tilesRight);
  countTiles(LEFT_SENSOR, tilesLeft);

  while (tilesRight < howFar && tilesLeft < howFar){    // While it has not gone back all the tiles  
    if ((tilesRight - tilesLeft) > TILES_DIFFERENCE){   // If the right wheel spins faster 
      motorDriver.speed(LMOTOR, 100);                   // Move left motor more than the right
      motorDriver.speed(RMOTOR, 85);
      countTiles(RIGHT_SENSOR, tilesRight);
      countTiles(LEFT_SENSOR, tilesLeft);

    }
    else if ((tilesLeft - tilesRight) > TILES_DIFFERENCE){
      motorDriver.speed(LMOTOR, 85);
      motorDriver.speed(RMOTOR, 100);
      countTiles(RIGHT_SENSOR, tilesRight);
      countTiles(LEFT_SENSOR, tilesLeft);

    }
    else{
      motorDriver.speed(RMOTOR, 100);  
      motorDriver.speed(LMOTOR, 100);
      countTiles(RIGHT_SENSOR, tilesRight);
      countTiles(LEFT_SENSOR, tilesLeft);

    }
  }
  resetTiles(tilesRight);                               // Reseting global variables
  resetTiles(tilesLeft);
}

/*
 * Purpose: counts the number of tiles read by sensor.
 * 
 * Inputs:  sensor to read from and global variable to be incremented.
 * Oututs:  NONE.
 */
int countTiles(int analogPin, int tilesVariable) {
  int currentState = blackOrWhite(analogPin);
  int previousState;
  
  if (currentState == BLACK && previousState == WHITE){
    tilesVariable++;
    previousState = currentState;
  }
  else if (currentState == WHITE && previousState == BLACK){
    tilesVariable++;
    previousState = currentState;
  }

  return tilesVariable;
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
 * Purpose: reset global variable used to count tiles. 
 */
void resetTiles(int globalTiles){
  globalTiles = 0;
}

int cmToTiles(int distance){
  float conversion = distance/ONE_TILE;
  conversion += 0.5;
  int tilesToObject = int(conversion);
  return tilesToObject;
}

// End of program
