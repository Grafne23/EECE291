#include "ColourSensing.h"
#include "Arduino.h"

int frequency = 0;
int count;
int redReading;
int greenReading;
int blueReading;
int sampleValues[NUM_SAMPLES];
int samples[NUM_SAMPLES];

int averageValues (int array[]){
  int index;
  int sum = 0;
  for(index = 0; index < NUM_SAMPLES-1; index++){
    sum += array[index];
  }
  return sum/NUM_SAMPLES;
}

int getAverageReadings(){
  for(count = 1; count < NUM_SAMPLES; count++){
    frequency = pulseIn(COLOUR_OUT, LOW);
    sampleValues[count] = frequency;
  }
  return averageValues(sampleValues);
}

void detectObjectColourAveraging(){
  int index;
  for(index = 0; index < NUM_SAMPLES; index++){
    samples[index] = detectObjectColour();
    delay(50);
  }
  
  float sum = 0;
  int red = 0, blue = 0, green = 0;
  for(index = 0; index < NUM_SAMPLES; index++){
    Serial.print(samples[index]);
    if(samples[index] == GREEN) green++;
    if(samples[index] == RED) red++;
    if(samples[index] == BLUE) blue++;
  }
  int colour;
  if(green >= red && green >= blue)
  {
    colour = GREEN;
  } else if (red >= green && red >= blue)
  {
    colour = RED;
  }
  else if (blue >= green && blue >= red)
  {
    colour = BLUE;
  }
  //int colour = (int) (sum / ((float) numberSamples));
  
  Serial.print("Object is "); Serial.println(colour);
  setColour(colour);
}


int detectObjectColour(){
    // ------------------- RED ---------------------------
  // Setting red filtered photodiodes to be read
  digitalWrite(COLOUR_PIN_S2,LOW);
  digitalWrite(COLOUR_PIN_S3,LOW);

  redReading = getAverageReadings();

  // ------------------- GREEN --------------------------
  // Setting Green filtered photodiodes to be read
  digitalWrite(COLOUR_PIN_S2,HIGH);
  digitalWrite(COLOUR_PIN_S3,HIGH);
  
  greenReading = getAverageReadings();
  
  // ------------------- BLUE ---------------------------
  // Setting Blue filtered photodiodes to be read
  digitalWrite(COLOUR_PIN_S2,LOW);
  digitalWrite(COLOUR_PIN_S3,HIGH);
  
  blueReading = getAverageReadings();
  Serial.print("red: "); Serial.println(redReading); 
  Serial.print("blue: "); Serial.println(blueReading); 
  Serial.print("green: "); Serial.println(greenReading); 

  if(redReading <= greenReading && redReading <= blueReading && redReading <= 10)
  {
    //Serial.println("Object is red"); 
    //setColour(RED); 
    return RED;
  }else if(redReading <= greenReading && blueReading <= greenReading -1)
  {
    //Serial.println("Object is blue");
    //setColour(BLUE);
    return BLUE;
  }else if(redReading >= 11 && greenReading >= 11 && blueReading > 11) 
  { 
    //Serial.println("Object is green");
    //setColour(GREEN);
    return GREEN;
  } else
  {
    Serial.println("invlid - cases need work");
  }
  return RED;
}

void setColour(int colour)
{
  switch(colour)
  {
    case RED:
      analogWrite(C_LED_PIN1, ON);
      analogWrite(C_LED_PIN2, ON);
      analogWrite(C_LED_PIN3, OFF);
      break;
    case BLUE:
      analogWrite(C_LED_PIN1, OFF);
      analogWrite(C_LED_PIN2, ON);
      analogWrite(C_LED_PIN3, ON);
      break;
    case GREEN:
      analogWrite(C_LED_PIN1, ON);
      analogWrite(C_LED_PIN2, OFF);
      analogWrite(C_LED_PIN3, ON);
      break;
    default:
      Serial.println("cannot display this colour!");
      break;
  }
}

