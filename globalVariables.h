#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H

//## Pinning##
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

//#FSM_CanRead#
volatile bool interrupt = false;
volatile bool mcp2515_overflow = false;
volatile bool arduino_overflow = false;
union my_can_msg {
  unsigned long value;
  unsigned char bytes[4];
};












#endif
