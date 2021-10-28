//##Libraries##
#include <DHT.h>
#include <Wire.h>
#include <OneWire.h>
#include <mcp2515.h>
#include "RTClib.h"
//##Header-Files##
#include "globalVariables.h"
#include "MOSFET.h"
#include "CAN1.h"
#include "CAN.h"
//#include "mcp2515.h"
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
//Objects Can Comm
volatile CanFrameStream cfStream;
MCP2515 mcp2515(11); //SS pin A3
//#RTC
RTC_DS1307 rtc;
void setup() {
  //#Objects
  Serial.begin(9600);
  dhtSensorTop.begin();
  dhtSensorOutside.begin();
  Wire.begin();
  SPI.usingInterrupt(InterruptPin);
  /*
  Serial.print("MCP RESET:ERROR:");
  Serial.println(mcp2515.reset());
  Serial.print("MCP Bitrate:ERROR:");
  Serial.println(mcp2515.setBitrate(CAN_1000KBPS, MCP_16MHZ));
  Serial.print("MCP Loopback:ERROR:");
  Serial.println(mcp2515.setLoopbackMode());
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }*/
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  DateTime now = rtc.now();
  Serial.print("Hour: ");
  Serial.println(now.hour());
  Serial.print("Minute: ");
  Serial.println(now.minute());
  Serial.print("Second: ");
  Serial.println(now.second());
  //#Pins
  attachInterrupt(InterruptPin, irqHandler, FALLING);
  pinMode(A3, OUTPUT);
}

void loop() {
  unsigned long currentTime = millis();
  if ( currentTime - previousTime >= systemPeriod ) {
    previousTime = currentTime;

    //#CAN
    canRead();

    //#State Machines
    FSM_CanRead();
    FSM_CirculationFan();
    FSM_FilterFan();
    FSM_LedsFan();
    FSM_MasterTimer();
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
