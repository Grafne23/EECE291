#include "ColourSensing.h"
#include "Arduino.h"

int frequency = 0;
int count;
int redReading;
int greenReading;
int blueReading;
int sampleValues[NUM_SAMPLES];
int samples[NUM_SAMPLES];

// Average the first NUM_SAMPLES values in the given array ---------------
int averageValues (int array[]){
  int index;
  int sum = 0;
  for(index = 0; index < NUM_SAMPLES; index++){
    sum += array[index];
  }
  return sum/NUM_SAMPLES;
}

// Take NUM_SAMPLES readings and average them---------------
int getAverageReadings(){
  for(count = 1; count < NUM_SAMPLES; count++){
    frequency = pulseIn(COLOUR_OUT, LOW);
    sampleValues[count] = frequency;
  }
  return averageValues(sampleValues);
}

// Detect the object colour ten times and set the LED based on the most common value ------
void detectObjectColourAveraging(){
  int index;
  int red = 0, blue = 0, green = 0;
  int colour;
  
  //Take NUM_SAMPLES readings
  for(index = 0; index < NUM_SAMPLES; index++){
    samples[index] = detectObjectColour();
    delay(50);
  }
  
  //Find the most common value
  for(index = 0; index < NUM_SAMPLES; index++){
    Serial.print(samples[index]);
    if(samples[index] == GREEN) green++;
    if(samples[index] == RED) red++;
    if(samples[index] == BLUE) blue++;
  }
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
  
  //Set the LED
  setColour(colour);
}

// Detect the object colour and return it ------
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

  if(redReading <= greenReading && redReading <= blueReading && redReading <= 10)
  {
    return RED;
  }else if(redReading <= greenReading && blueReading <= greenReading -1)
  {
    return BLUE;
  }else if(redReading >= 11 && greenReading >= 11 && blueReading > 11) 
  { 
    return GREEN;
  } else
  {
    Serial.println("invlid - cases need work");
  }
  return RED;
}

// Set the colour of the tri-colour LED ------
// Any value other than RED(0) GREEN(1) BLUE(2) for off
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
      analogWrite(C_LED_PIN1, ON);
      analogWrite(C_LED_PIN2, ON);
      analogWrite(C_LED_PIN3, ON);
      break;
  }
}

