#include "DistanceSensing.h"
#include "Arduino.h"

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
 delayMicroseconds(5);
 digitalWrite(ULTRA_SENSOR, LOW);

  //read ultrasonic sensor, convert duration of pulse to centimeters
  pinMode(ULTRA_SENSOR, INPUT);
  duration = pulseIn(ULTRA_SENSOR, HIGH);
  //Serial.print("Duration: ");Serial.println(duration);
  distance_cm = duration/29/2;

  if(distance_cm > MAX_DISTANCE)
  {
    return -1;
  }

  return distance_cm;
}

