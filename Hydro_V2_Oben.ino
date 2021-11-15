//##Libraries##
#include <DHT.h>
#include <Wire.h>
#include <OneWire.h>
#include <mcp2515.h>
#include <RTClib.h>
#include <EEPROM.h>
//##Header-Files##
#include "globalVariables.h"
#include "MOSFET.h"
#include "CAN1.h"
//##Object intialization##
//#TempHum#
#define DHTTYPE DHT22
DHT dhtSensorTop(TempHumTop, DHTTYPE);
DHT dhtSensorOutside(TempHumOutside, DHTTYPE);
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
void setup() {
  //#Objects
  Serial.begin(2000000);
  dhtSensorTop.begin();
  dhtSensorOutside.begin();
  Wire.begin();
  SPI.usingInterrupt(InterruptPin);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS, MCP_16MHZ);
  mcp2515.setNormalMode();
  //mcp2515.setLoopbackMode();

    /*
      if (! rtc.isrunning()) {
      Serial.println("RTC is NOT running!");
      // following line sets the RTC to the date & time this sketch was compiled
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      }*/
    rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  if (DEBUG) {
    DateTime now = rtc.now();
    Serial.print("Hour: ");
    Serial.println(now.hour());
    Serial.print("Minute: ");
    Serial.println(now.minute());
    Serial.print("Second: ");
    Serial.println(now.second());
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
    //masterDummy();

  }
}

void masterDummy() {
  if (Serial.available() > 0) {
    int message = Serial.parseInt();
    Serial.println(message);
    if (message == 1) {
      message = 0;
      testFlag = true;
    }
  }
};
