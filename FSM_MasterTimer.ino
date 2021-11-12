void FSM_MasterTimer() {

  switch (masterTimerState) {
    case masterTimerStates::Init:
      if (DEBUG)
        Serial.println("Init");
      //Retrieve last daytime snapshot from EEPROM
      a = EEPROM.read(BYT3);
      b = EEPROM.read(BYT2);
      c = EEPROM.read(BYT1);
      d = EEPROM.read(BYT0);
      daytimeSnap = eepromToInt(a, b, c, d);
      uint32_t dif = (now.unixtime() - daytimeSnap) % dayInSec; //Calculate difference nowtime vs daytimeSnap, Modulo in case system was off for longer than a day
      sampletimeSnap = now.unixtime() - (dif%(30*minInSec)); // Same as pumptime
      //here the phase and pumpcycle should also be retrieved from EEPROM
      if (dif < (daytimeDuration[phase].h*hourInSec + daytimeDuration[phase].m*minInSec)) { 
        masterTimerState = masterTimerStates::DayTimer;
        daytime = day;
        Serial.println("DayTimer");
        canWrite(2, 1);
        ledsFanOnFlag = true;
        pumptimeSnap = now.unixtime() - ((dif+10*minInSec)%(pumpInterval*hourInSec)); //Calculate pumptimeSnap, by first calculating seconds passed since the last pumptime and then subtracting that value from nowtime. 10 min shift bcs of pumptime shift 
      } else {
        masterTimerState = masterTimerStates::NightTimer;
        daytime = night;
        Serial.println("NightTimer");
      }
      break;
    case masterTimerStates::DayTimer:
      if (DEBUG)
        //Serial.println("DayTimer");
      if (now.unixtime() - pumptimeSnap >= pumpInterval*hourInSec){
        pumptimeSnap = now.unixtime();
        pumpTimingFlag = true;
      }
      if (now.unixtime()-sampletimeSnap >= 30*minInSec){
        sampletimeSnap = now.unixtime();
        sensorTimingFlag = true;
      }
      if (now.unixtime()-daytimeSnap >= daytimeDuration[phase].h*hourInSec + daytimeDuration[phase].m*minInSec) {
        ledsOffTimingFlag = true;
        //sensorTimingFlag = false; so as not to overwrite sensor command
        pumpTimingFlag = false;
      }

      if (pumpTimingFlag) {
        masterTimerState = masterTimerStates::CmdPump;
        pumpTimingFlag = false;
      } else if (sensorTimingFlag) {
        masterTimerState = masterTimerStates::CmdSensors;
        sensorTimingFlag = false;
      } else if (ledsOffTimingFlag) {
        masterTimerState = masterTimerStates::CmdLightOff;
        ledsOffTimingFlag = false;
        return;
      }
      break;
    case masterTimerStates::NightTimer:
      if (DEBUG)
        Serial.println("NightTimer");
        
      if (now.hour() == hPump && now.minute() == mPump && pumpBlock == false) {
        pumpBlock = true;
        pumpTimingFlag = true;
      }
      if (now.unixtime()-sampletimeSnap >= 30*minInSec){
        sampletimeSnap = now.unixtime();
        sensorTimingFlag = true;
      }
      if (now.hour() == hOn && now.minute() == mOn) {
        ledsOnTimingFlag = true;
        //sensorTimingFlag = false;
        pumpTimingFlag = false;
      }

      if (pumpTimingFlag) {
        masterTimerState = masterTimerStates::CmdPump;
        pumpTimingFlag = false;
      } else if (sensorTimingFlag) {
        masterTimerState = masterTimerStates::CmdSensors;
        sensorTimingFlag = false;
      } else if (ledsOnTimingFlag) {
        masterTimerState = masterTimerStates::CmdLightOn;
        ledsOnTimingFlag = false;
        return;
      }
      break;
    case masterTimerStates::CmdLightOn:
      if (DEBUG) {
        Serial.println("CmdLightOn");
      }
      //Here the light intensity can be transfered via data field once hardware makes it possible
      canWrite(2, 1);
      ledsFanOnFlag = true;
      daytime = day;
      pumpBlock = false;
      phase = uiPhase;
      pumpInterval = uiPumpInterval;
      masterTimerState = masterTimerStates::DayTimer;
      return;
    case masterTimerStates::CmdLightOff:
      if (DEBUG) {
        Serial.println("CmdLightOff");
      }
      canWrite(2, 0);
      ledsFanOnFlag = false;
      daytime = night;
      masterTimerState = masterTimerStates::NightTimer;
      return;
    case masterTimerStates::CmdPump:
      canWrite(1);
      if (DEBUG)
        Serial.println("CmdPump");
      if (daytime == day)
        masterTimerState = masterTimerStates::DayTimer;
      else
        masterTimerState = masterTimerStates::NightTimer;
      break;
    case masterTimerStates::CmdSensors:
      if (DEBUG)
        Serial.println("CmdSensors");
      canWrite(0);
      sampleFlagTop = true;
      if (daytime == day)
        masterTimerState = masterTimerStates::DayTimer;
      else
        masterTimerState = masterTimerStates::NightTimer;
      break;
    default:
      Serial.println("Error: FSM_MasterTimer()");
  }
}

uint32_t eepromToInt(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
  uint32_t tempI = (a << 24) | (b << 16) | (c << 8) | d;
  return tempI;
}

uint8_t splitInt(uint32_t intData, uint8_t byt) {
  if ( byt == BYT0) {
    return (intData & 0xFF);
  } else if (byt == BYT1) {
    return ((intData >> 8) & 0xFF);
  } else if (byt == BYT2) {
    return ((intData >> 16) & 0xFF);
  } else if (byt == BYT3) {
    return ((intData >> 24) & 0xFF);
  }
  else {                                          //Just for testing purposes
    Serial.println("splitInt() wrong byt input!!!!");
    return 0;
  }
}
