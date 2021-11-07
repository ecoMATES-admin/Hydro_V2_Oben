void FSM_MasterTimer() {

  DateTime now = rtc.now();

  switch (masterTimerState) {
    case masterTimerStates::Init:
      if (DEBUG)
        Serial.println("Init");
      if (now.hour() >= hOn  && now.hour() <= lightOff[phase].h && now.minute() < lightOff[phase].m) {
        masterTimerState = masterTimerStates::DayTimer;
        daytime = day;
        Serial.println("DayTimer");
        canWrite(2, 1);
      ledsFanOnFlag = true;
      } else {
        masterTimerState = masterTimerStates::NightTimer;
        daytime = night;
        Serial.println("NightTimer");
      }
      break;
    case masterTimerStates::DayTimer:
      if (DEBUG)
        //Serial.println("DayTimer");
      if (pumpBlock = true){ //pump command debounce
        pumpBlockCounter++;
        if (pumpBlockCounter>=7000){
          pumpBlock = false;
          pumpBlockCounter = 0;
        }
      }
      if (sensorBlock = true){ //sensor command debounce
        sensorBlockCounter++;
        if (sensorBlockCounter>=7000){
          sensorBlock = false;
          sensorBlockCounter = 0;
        }
      }
      if (now.hour() == pumpTimes[pumpIterator*pumpInterval-1].h && now.minute() == pumpTimes[pumpIterator*pumpInterval-1].m && pumpBlock ==false){  // every 2h but 10 minutes shifted so that last watering happens 10 min before the light is turned off
        pumpTimingFlag = true;
        pumpBlock=true;
        pumpIterator++;
      }
      if ((now.minute() == 0 || now.minute() == 30) && sensorBlock == false){ 
        sensorTimingFlag = true;
        sensorBlock = true;
      }
      if (now.hour() == lightOff[phase].h && now.minute() == lightOff[phase].m) {
        ledsOffTimingFlag = true;
        //sensorTimingFlag = false; so as not to overwrite sensor command
        pumpTimingFlag = false;
        pumpIterator = 1;
        pumpBlock = false;
        pumpBlockCounter = 0;
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
        //Serial.println("NightTimer");
      if (sensorBlock = true){ //sensor command debounce
        sensorBlockCounter++;
        if (sensorBlockCounter>=7000){
          sensorBlock = false;
          sensorBlockCounter = 0;
        }
      } 
      if (now.hour() == hPump && now.minute() == mPump && pumpBlock ==false){ 
        pumpBlock = true;
        pumpTimingFlag = true;
      }
      if ((now.minute() == 0 || now.minute() == 30) && sensorBlock == false){ 
        sensorTimingFlag = true;
        sensorBlock = true;
      }
      if (now.hour() == hOn && now.minute() == mOn) {
        ledsOnTimingFlag = true;
        //sensorTimingFlag = false;
        pumpBlock = false;
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
      if (DEBUG){
        Serial.println("CmdLightOn");}
      //An dieser Stelle mithilfe von switch maximale Lichtintensität abhängig von Phase übergeben
      canWrite(2, 1);
      ledsFanOnFlag = true;
      daytime = day;
      phase = uiPhase;
      masterTimerState = masterTimerStates::DayTimer;
      return;
    case masterTimerStates::CmdLightOff:
      if (DEBUG){
        Serial.println("CmdLightOff");}
      canWrite(2, 0);
      ledsFanOnFlag = false;
      daytime = night;
      masterTimerState = masterTimerStates::NightTimer;
      return;
    case masterTimerStates::CmdPump:
      canWrite(1);
      if (DEBUG){
        Serial.println("CmdPump");
      }
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
