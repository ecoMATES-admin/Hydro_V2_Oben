//##Libraries##
#include <DHT.h>
#include <Wire.h>
#include <OneWire.h>
#include <mcp2515.h>
//##Header-Files##
#include "globalVariables.h"
#include "MOSFET.h"
#include "CAN.h"
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
MCP2515 mcp2515(A3); //SS pin A3

void setup() {
  //#Objects
  Serial.begin(9600);
  dhtSensorTop.begin();
  dhtSensorOutside.begin();
  Wire.begin();
  SPI.usingInterrupt(0);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS, MCP_16MHZ);
  mcp2515.setNormalMode();
  //#Pins
  attachInterrupt(0, irqHandler, FALLING);
}

void loop() {
  unsigned long currentTime = millis();
  if ( currentTime - previousTime >= systemPeriod ) {
    previousTime = currentTime;

    //#State Machines
    //FSM_CirculationFan();
    FSM_CanRead();
    masterDummy();

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
