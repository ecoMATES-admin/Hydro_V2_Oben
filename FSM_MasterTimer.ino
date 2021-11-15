void FSM_MasterTimer() {
  //EEPROM write missing
  uint32_t daytimeSec;
  switch (masterTimerState) {
    case masterTimerStates::Init:
      if (DEBUG)
        Serial.println("Init");
      //Retrieve last daytime snapshot from EEPROM
      EEPROM.get(BYT0, daytimeSnap);
      if (DEBUG) {
        Serial.println("daytimeSnap");
        Serial.println(daytimeSnap);
        Serial.println("now.unixtime()");
        Serial.println(now.unixtime());
      }
      dif = (now.unixtime() - daytimeSnap) % dayInSec; //Calculate difference nowtime vs daytimeSnap, Modulo in case system was off for longer then a day
      sampletimeSnap = now.unixtime() - (dif % (sampleTime * minInSec)); // Same as pumptime
      if (DEBUG) {
        Serial.println("dif");
        Serial.println(dif);
        Serial.println("sampletimeSnap");
        Serial.println(sampletimeSnap);
      }
      //here the phase and pumpcycle should also be retrieved from EEPROM
      daytimeSec= daytimeDuration[phase].h * hourInSec + daytimeDuration[phase].m * minInSec;
      if (DEBUG) {
        Serial.println("daytimeSec");
        Serial.println(daytimeSec);
      }
      if (dif < daytimeSec) {
        masterTimerState = masterTimerStates::DayTimer;
        daytime = day;
        Serial.println("DayTimer");
        canWrite(2, 1);
        ledsFanOnFlag = true;
        pumptimeSnap = now.unixtime() - ((dif + 10 * minInSec) % (pumpInterval * hourInSec)); //Calculate pumptimeSnap, by first calculating seconds passed since the last pumptime and then subtracting that value from nowtime. 10 min shift bcs of pumptime shift
        if (DEBUG) {
        Serial.println("pumptimeSnap");
        Serial.println(pumptimeSnap);
      }
      } else {
        masterTimerState = masterTimerStates::NightTimer;
        daytime = night;
        Serial.println("NightTimer");
      }
      break;
    case masterTimerStates::DayTimer:
      if (false)
        Serial.println("DayTimer");
      if (now.unixtime() - pumptimeSnap >= pumpInterval * hourInSec) {
        pumptimeSnap = now.unixtime();
        pumpTimingFlag = true;
      }
      if (now.unixtime() - sampletimeSnap >= sampleTime * minInSec) {
        sampletimeSnap = now.unixtime();
        sensorTimingFlag = true;
      }
      if (now.unixtime() - daytimeSnap >= (daytimeDuration[phase].h * hourInSec + daytimeDuration[phase].m * minInSec)) {
        ledsOffTimingFlag = true;
        //sensorTimingFlag = false; so as not to overwrite sensor command
        pumpTimingFlag = false;
      }

      if (ledsOffTimingFlag) {
        masterTimerState = masterTimerStates::CmdLightOff;
        ledsOffTimingFlag = false;
        break;
      } else if (sensorTimingFlag) {
        masterTimerState = masterTimerStates::CmdSensors;
        sensorTimingFlag = false;
      } else if (pumpTimingFlag) {
        masterTimerState = masterTimerStates::CmdPump;
        pumpTimingFlag = false;
      }
      break;
    case masterTimerStates::NightTimer:
      if (false)
        Serial.println("NightTimer");

      if (now.hour() == hPump && now.minute() == mPump && pumpBlock == false) {
        pumptimeSnap = now.unixtime();
        pumpBlock = true;
        pumpTimingFlag = true;
      }
      if (now.unixtime() - sampletimeSnap >= sampleTime * minInSec) {
        sampletimeSnap = now.unixtime();
        sensorTimingFlag = true;
      }
      if (now.hour() == hOn && now.minute() == mOn) {
        ledsOnTimingFlag = true;
        //sensorTimingFlag = false;
        pumpTimingFlag = false;
      }

      if (ledsOnTimingFlag) {
        masterTimerState = masterTimerStates::CmdLightOn;
        ledsOnTimingFlag = false;
        break;
      } else if (sensorTimingFlag) {
        masterTimerState = masterTimerStates::CmdSensors;
        sensorTimingFlag = false;
      } else if (pumpTimingFlag) {
        masterTimerState = masterTimerStates::CmdPump;
        pumpTimingFlag = false;
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
      //EEPROM write daytimeSnap
      EEPROM.put(0, now.unixtime());
      daytimeSnap = now.unixtime();
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
      canWrite(0);
      if (DEBUG)
        Serial.println("CmdSensors");
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
