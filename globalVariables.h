#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H

//## Pinning##
#define InterruptPin 0 //Arduino pin 2
//#Sensors
#define TempHumTop 6
#define TempHumOutside 7
//#Actuators
#define FanFilter 8
#define FanLeds 9
#define FanCirculation 10

#define MSB 1
#define LSB 0

//##Variables##
//#Debug#
#define DEBUG 0
bool testFlag = false;
//#SystemClock#
unsigned long previousTime = 0; 
unsigned long systemPeriod = 10; // milliseconds
//#SystemTiming#
unsigned long timer = 0;
bool pumpBlock = false; //pump is triggered byrtc in nighttime, to prevent multiple pump commands pumpBlock is implemented
int pumpBlockCounter = 0;
bool sensorBlock = false;
int sensorBlockCounter = 0;

//MasterTimer
bool ledsOnTimingFlag= false;
bool ledsOffTimingFlag = false;
bool pumpTimingFlag = false;
bool sensorTimingFlag = false;
//Peripherals
bool sampleFlagTop = false;
bool circulationFanOnFlag = true;
bool ledsFanOnFlag = false;
const bool filterFlagOnFlag = true;

//#Sensor data #
float tempHumValBottom[2] = {0};
float tempHumValTop[2] = {0};
float tempHumValOutside[2] = {0};
float waterTempVal = 0;
float phVal = 0;
float ecVal = 0;
float waterLevelVal = 0;

//#FSM_Sensordata#
uint8_t readCounter = 0; //timer for ReadValues State
float humError = 199.0, tempError = 99.0;
float tempHumTop[2] = {0};
float tempHumOutside[2] = {0};

enum class sensorStates:uint8_t{
  Idle, ReadTop,ReadOutside
};
sensorStates sensorState =sensorStates::Idle;

//#FSM_CanRead#
volatile bool interrupt = false;
volatile bool mcp2515_overflow = false;
volatile bool arduino_overflow = false;
union my_can_msg {
  unsigned long value;
  unsigned char bytes[4];
};

//#FSM_CirculationFan#
enum class circulationFanStates:uint8_t{
  FanOn, FanOff
};
circulationFanStates circulationFanState = circulationFanStates::FanOn;

//#FSM_LedsFan#
enum class ledsFanStates:uint8_t{
  FanOn, FanOff
};
ledsFanStates ledsFanState = ledsFanStates::FanOff;

//#FSM_FilterFan#
enum class filterFanStates:uint8_t{
  FanOn
};
filterFanStates filterFanState = filterFanStates::FanOn;

//#FSM_MasterTimer#
//Light
struct timeStamp{
  int h,m;
};
//int hOn = 8, mOn = 0;
int hOn = 18, mOn = 33; //Test purposes
timeStamp lightOff[4] ={
  {20,0},
  {2,0},
  {20,0},
  {19,30} //Test purposes
};
enum daytimes:uint8_t{
  day, night
};
daytimes daytime = night;
enum phases:uint8_t{
  sprout, veggie, flower,test
};
phases phase = test; //phase needs to be stored in EEPROM
phases uiPhase = test;
//Pump
int hPump = 18, mPump =23;
enum pumpIntervals:uint8_t{
   h1=1, h2=2, h4=4, h6=6
};
pumpIntervals pumpInterval = h2;
struct pumpTime{
  uint8_t h,m;
};
uint8_t pumpIterator = 1;
pumpTime pumpTimes[18] ={ //cyle/h
  {8,50},
  {9,50},
  {10,50},
  {11,50},
  {12,50},
  {13,50},
  {14,50},
  {15,50},
  {16,50},
  {17,50},
  {18,50},
  {19,50},
  {20,50},
  {21,50},
  {22,50},
  {23,50},
  {0,50},
  {1,50},
};


enum class masterTimerStates:uint8_t{
  Init,DayTimer, NightTimer, CmdLightOn, CmdLightOff, CmdPump, CmdSensors
};
masterTimerStates masterTimerState = masterTimerStates::Init;


#endif
