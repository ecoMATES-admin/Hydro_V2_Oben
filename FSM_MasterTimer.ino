void FSM_MasterTimer() {

  DateTime now = rtc.now();

  switch (masterTimerState) {
    case masterTimerStates::Init:
      if (DEBUG)
        Serial.println("Init");
      if (now.hour() >= hOn && now.minute() >= mOn && now.hour() <= lightOff[phase].h && now.minute() < lightOff[phase].m) {
        masterTimerState = masterTimerStates::DayTimer;
        daytime = day;
      } else {
        masterTimerState = masterTimerStates::NightTimer;
        daytime = night;
      }
      break;
    case masterTimerStates::DayTimer:
      if (DEBUG)
        Serial.println("DayTimer");
      if ((timer+ 100*60*10)% (100  * 60* 60 * pumpTime)== 0)  // every 2h but 10 minutes shifted so that last watering happens 10 min before the light is turned off
        pumpTimingFlag = true;
      if (timer % (100  * /*60 * */ 30) == 0) // every 30 min with f,Sys= 100 Hz !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        sensorTimingFlag = true;
      if (now.hour() == lightOff[phase].h && now.minute() == lightOff[phase].m) {
        ledsOffTimingFlag = true;
        sensorTimingFlag = false;
        pumpTimingFlag = false;
        timer = 0;
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
      if (now.hour() == hPump && now.minute() == mPump && !pumpBlock) // just intermediate solution till light ramp is working
        pumpBlock = true;
        pumpTimingFlag = true;
      if (timer % (100 * /*60 * */30) == 0) // every 30 min with f,Sys= 100 Hz   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        sensorTimingFlag = true;
      if (now.hour() == hOn && now.minute() == mOn) {
        ledsOnTimingFlag = true;
        pumpBlock = false;
        timer = 0;
        sensorTimingFlag = false;
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
      if (DEBUG)
        Serial.println("CmdLightOn");
      //An dieser Stelle mithilfe von switch maximale Lichtintensität abhängig von Phase übergeben
      canWrite(2, 1);
      ledsFanOnFlag = true;
      daytime = day;
      phase = uiPhase;
      masterTimerState = masterTimerStates::DayTimer;
      return;
    case masterTimerStates::CmdLightOff:
      if (DEBUG)
        Serial.println("CmdLightOff");
      canWrite(2, 0);
      ledsFanOnFlag = false;
      daytime = night;
      masterTimerState = masterTimerStates::NightTimer;
      return;
    case masterTimerStates::CmdPump:
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
  timer++;
}
