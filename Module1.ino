#include "MotorDriver.h"

#define RMOTOR 0
#define LMOTOR 1

MotorDriver motorDriver;

void setup() {
  // put your setup code here, to run once:
  motorDriver.begin();
}

void loop() {
 /*
  int i;
  for(i = 10; i > 0; i--)
  {
    motorDriver.speed(RMOTOR, 10*i);
    delay(1000);
  }
  */
  motorDriver.speed(LMOTOR, 40);  
  delay(3000);
  motorDriver.stop(LMOTOR);
  delay(100);
  motorDriver.speed(RMOTOR, 40);
  delay(3000);
  motorDriver.stop(RMOTOR);
  delay(100);
  /*
  motorDriver.speed(RMOTOR, 80);
  delay(1000);
  motorDriver.speed(RMOTOR, 60);
  delay(1000);
  */

  delay(8000);
}
