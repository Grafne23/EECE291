#include "DistanceSensing.h"
#include "Arduino.h"

// Read the distance and return it in cm or -1 if no object detected -------
long getDistance()
{
 /* ultrasonic sensor is triggered by a HIGH pulse of 2 or more microseconds.
  Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  */
 long duration, distance_cm;   
    
 pinMode(ULTRA_SENSOR, OUTPUT);
 digitalWrite(ULTRA_SENSOR, LOW);
 delayMicroseconds(2);
 digitalWrite(ULTRA_SENSOR, HIGH);
 delayMicroseconds(2);
 digitalWrite(ULTRA_SENSOR, LOW);

  //read ultrasonic sensor, convert duration of pulse to centimeters
  pinMode(ULTRA_SENSOR, INPUT);
  duration = pulseIn(ULTRA_SENSOR, HIGH);
  distance_cm = duration/29/2;

  if(distance_cm > MAX_DISTANCE)
  {
    //return -1 for no object detected
    return -1;
  }

  return distance_cm;
}

