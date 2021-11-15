#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H

//## Pinning##
#define InterruptPin 0 //Arduino pin 2
//#Bus
#define SW_TX 3
#define SW_RX 4
#define CAN_CS A3
//#Sensors
#define TempHumTop 6
#define TempHumOutside 7
//#Actuators
#define FanFilter 8
#define FanLeds 9
#define FanCirculation 10
//#Display
#define BUTTON A0

//uint32_t = BYT3 BYT2 BYT1 BYT0
//uint16_t = BYT1 BYT0
//#define BYT3 3
//#define BYT2 2
#define BYT1 1
#define BYT0 0

//##Variables##
//#Debug#
#define DEBUG 1
bool testFlag = false;
//#SystemClock#
unsigned long previousTime = 0; 
unsigned long systemPeriod = 10; // milliseconds

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
const uint32_t dayInSec = 86400;
const uint16_t hourInSec = 3600;
const uint8_t minInSec = 60;
uint32_t daytimeSnap = 0;
uint32_t pumptimeSnap = 0;
uint32_t sampletimeSnap = 0;
uint32_t dif=0;
bool pumpBlock = false;
//Sensors
uint8_t sampleTime = 1;
//Light
struct timeStamp{
  int h,m;
};
int hOn = 18, mOn = 41; //Test purposes
timeStamp daytimeDuration[4] ={
  {12,0},
  {18,0},
  {12,0},
  {0,2} //Test purposes
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
int hPump = 18, mPump =4;
enum pumpIntervals:uint8_t{
   h1=1, h2=2, h3=3, h6=6
};
pumpIntervals pumpInterval = h1;
pumpIntervals uiPumpInterval = h1;
struct pumpTime{
  uint8_t h,m;
};
enum class masterTimerStates:uint8_t{
  Init,DayTimer, NightTimer, CmdLightOn, CmdLightOff, CmdPump, CmdSensors
};
masterTimerStates masterTimerState = masterTimerStates::Init;

#endif
