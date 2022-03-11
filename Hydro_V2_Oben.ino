//##Libraries##
#include <Wire.h>
#include <SoftwareSerial.h>
#include <mcp2515.h>
#include <RTClib.h>
#include <EEPROM.h>
#include <Adafruit_SHT31.h>

//##Header-Files##
#include "globalVariables.h"
#include "MOSFET.h"
#include "CAN1.h"

//##Object intialization##
//#TempHum#
Adafruit_SHT31 shtTop = Adafruit_SHT31();
Adafruit_SHT31 shtOutside = Adafruit_SHT31();

//#Mosfet
MOSFET fanCirculation(FanCirculation);
MOSFET fanFilter(FanFilter);
MOSFET fanLeds(FanLeds);
//#Can
volatile CanBuffer canBuffer;
volatile CanFloatBuffer canFloatBuffer;
volatile CanFrameStream cfStream;
MCP2515 mcp2515(CAN_CS); //SS pin A3
//#RTC
RTC_DS1307 rtc;
DateTime now;
//SoftwareSerial
SoftwareSerial NodeSerial(3, 4); //(rx,tx)

//##SETUP##
void setup() {
  //#Object begins
  Serial.begin(2000000);
  Wire.begin();
  if (! shtTop.begin(0x45) && DEBUG)   
    Serial.println("Couldn't find shtTop");
  if (! shtOutside.begin(0x44) && DEBUG)   
    Serial.println("Couldn't find shtOutside");
  shtTop.heater(false);
  if (shtTop.isHeaterEnabled()){
    Serial.println("shtTop heater is ON");
  }
  shtOutside.heater(false);
  if (shtOutside.isHeaterEnabled()){
    Serial.println("shtOutside heater is ON");
  }
  //#Can
  SPI.usingInterrupt(InterruptPin);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS, MCP_16MHZ);
  mcp2515.setNormalMode();
  NodeSerial.begin(115200);
  //#RTC
  rtc.begin();
  if (DEBUG) {
    DateTime now = rtc.now();
    Serial.println(now.timestamp(DateTime::TIMESTAMP_FULL));
    Serial.println("dayInSec");
    Serial.println(dayInSec);
    Serial.println("hourInSec");
    Serial.println(hourInSec);
    Serial.println("minInSec");
    Serial.println(minInSec);
  }
  //#Pins
  attachInterrupt(InterruptPin, irqHandler, FALLING);
  pinMode(A3, OUTPUT);
}

void loop() {
  unsigned long currentTime = millis();
  if ( currentTime - previousTime >= systemPeriod ) {
    previousTime = currentTime;
    //Serial.println(previousTime);
    now = rtc.now();

    //#CAN
    canRead();

    //#State Machines
    FSM_CanRead();
    FSM_CirculationFan();
    FSM_FilterFan();
    FSM_LedsFan();
    FSM_MasterTimer();
    FSM_Sensordata();
    FSM_DataLog();
  }
}
