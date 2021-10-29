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
#define DEBUG 1
bool testFlag = false;
//#SystemClock#
unsigned long previousTime = 0; 
unsigned long systemPeriod = 10; // milliseconds
//#SystemTiming#
unsigned long timer = 0;
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
uint8_t dthTopCounter = 0;
uint8_t dthOutsideCounter = 0;
int n = 250;
float tempHumTop[2] = {0};
float tempHumOutside[2] = {0};
float tempHumTopMean[2] = {0};
float tempHumOutsideMean[2] = {0};
enum class sensorStates:uint8_t{
  Idle, ReadValues,CalculateMean
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
enum class masterTimerStates:uint8_t{
  CheckTime, cmdLightOn, cmdLightOff, cmdPump, cmdSensors
};
masterTimerStates masterTimerState = masterTimerStates::CheckTime;









#endif
