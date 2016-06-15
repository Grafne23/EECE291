#define RMOTOR 0                             //right motor pin
#define LMOTOR 1                             //left motor pin
#define RSENSOR 0x0                          //right ir line sensor address
#define LSENSOR 0x1                          //left ir line sensor address
#define CSENSOR A0

//ir line sensor modes
#define SENSOR_ADDR_OFF_OFF (0x4B)           
#define SENSOR_ADDR_OFF_ON (0x49) 
#define SENSOR_ADDR_ON_OFF (0x4A) 
#define SENSOR_ADDR_ON_ON (0x48) 

const uint8_t sensorAddr = SENSOR_ADDR_ON_OFF;  //ir line sensor mode

#define LINETHRESHOLD 235
#define CENTERTHRESHOLD 33

void followLine(int rOn, int lOn, MotorDriver motorDriver);
void followLineBackwards(int rOn, int lOn, MotorDriver motorDriver);
void readLineSensor(int sensorByte, int *rOn, int *lOn);
void readLineSensors(int *rOn, int *lOn);
int ReadByte(uint8_t addr, uint8_t reg, uint8_t *data); 
void WriteByte(uint8_t addr, uint8_t reg, byte data);
void lookForLine(MotorDriver motorDriver);

