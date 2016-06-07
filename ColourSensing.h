#define COLOUR_PIN_S0_S1 1
//#define COLOUR_PIN_S1 2
#define COLOUR_PIN_S2 4
#define COLOUR_PIN_S3 7

#define C_LED_PIN1  3
#define C_LED_PIN2  5
#define C_LED_PIN3  6

#define ON  255
#define OFF 0

#define COLOUR_OUT 0

#define NUM_SAMPLES 5

#define RED 0
#define BLUE 1
#define GREEN 2
#define NO_COLOUR 3

int averageValues (int array[]);
int getAverageReadings();
int detectObjectColour();
void detectObjectColourAveraging();
void setColour(int colour);

