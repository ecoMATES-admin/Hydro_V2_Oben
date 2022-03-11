#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H

//## Pinning##
#define InterruptPin 0 //Arduino pin 2
//#Bus
#define SW_TX 4
#define SW_RX 3
#define CAN_CS A3
//#Actuators
#define FanFilter 8
#define FanLeds 10
#define FanCirculation 9
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
//bool testFlag = false;
//#SystemClock#
unsigned long previousTime = 0; 
unsigned long systemPeriod = 10; // milliseconds

//#Flags#
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
//Data Logging
uint8_t sendAllFlag = 0;
bool sendWaterFlag = false;

//#Sensor data#
float tempHumValBottom[2] = {0};
float tempHumValTop[2] = {0};
float tempHumValOutside[2] = {0};
float waterTempVal = 0;
float phVal = 0;
float ecVal = 0;
float waterLevelVal = 0;

//#FSM_Sensordata#
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

//#FSM_DataLog#
char dataBuffer[6] = {0};
enum class dataLogStates:uint8_t{
  Idle, SendAll, SendWater
};
dataLogStates dataLogState = dataLogStates::Idle;

//#FSM_MasterTimer#
//Timing
const uint32_t dayInSec = 86400;
const uint16_t hourInSec = 3600;
const uint8_t minInSec = 60;
uint32_t daytimeSnap = 0;
uint32_t pumptimeSnap = 0;
uint32_t sampletimeSnap = 0;
uint32_t dif=0;
bool pumpBlock = false;
//Sensors
uint8_t sampleTime = 15 ; //in sec, sampleTime = {2,15,30} //2 sec ist sehr wohl ein Problem denn gepumpt wird um 50 nach....
                                                           //bzw muss man 2 potentielle Probleme festhalten: Messen während gepumpt wird & gleichzeitiges hochladen von Wasserpegel und Rest
//Light
enum phases:uint8_t{
  sprout, veggie, flower
};
phases phase = sprout; //phase needs to be stored in EEPROM
phases uiPhase = sprout;
enum daytimes:uint8_t{
  day, night
};
daytimes daytime = night;
struct timeStamp{
  uint8_t h,m;
};
timeStamp daytimeDuration[3] ={
  {12,0},
  {18,0},
  {12,0},
  //{0,2} //Test purposes
};
int hOn = 8, mOn = 0; 
//Pump
int hPump = 7, mPump =50;
enum pumpIntervals:uint8_t{
   h1=1, h2=2, h3=3, h6=6
};
pumpIntervals pumpInterval = h2;
pumpIntervals uiPumpInterval = h2;

//States
enum class masterTimerStates:uint8_t{
  Init,DayTimer, NightTimer, CmdLightOn, CmdLightOff, CmdPump, CmdSensors
};
masterTimerStates masterTimerState = masterTimerStates::Init;

#endif
