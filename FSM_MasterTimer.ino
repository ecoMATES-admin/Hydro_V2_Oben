void FSM_MasterTimer() {
  timer++;
  switch (masterTimerState) {
    case masterTimerStates::CheckTime:
      if (timer == 1000)
        ledsOnTimingFlag = true;
      if (timer == 3000)
        pumpTimingFlag = true;
      if (timer % 2000 == 0)
        sensorTimingFlag = true;
      if (timer == 42000) {
        ledsOffTimingFlag = true;
        timer = 0;
      }
      if (ledsOnTimingFlag) {
        masterTimerState = masterTimerStates::cmdLightOn;
        ledsOnTimingFlag = false;
      } else if (pumpTimingFlag) {
        masterTimerState = masterTimerStates::cmdPump;
        pumpTimingFlag = false;
      } else if (sensorTimingFlag) {
        masterTimerState = masterTimerStates::cmdSensors;
        sensorTimingFlag = false;
      } else if (ledsOffTimingFlag) {
        masterTimerState = masterTimerStates::cmdLightOff;
        ledsOffTimingFlag = false;
      }
      break;
    case masterTimerStates::cmdLightOn:
      canWrite(2, 1);
      ledsFanOnFlag = true;
      masterTimerState = masterTimerStates::CheckTime;
      break;
    case masterTimerStates::cmdLightOff:
      canWrite(2, 0);
      ledsFanOnFlag = false;
      masterTimerState = masterTimerStates::CheckTime;
      break;
    case masterTimerStates::cmdPump:
      canWrite(1);
      masterTimerState = masterTimerStates::CheckTime;
      break;
    case masterTimerStates::cmdSensors:
      canWrite(0);
      masterTimerState = masterTimerStates::CheckTime;
      break;
    default:
      Serial.println("Error: FSM_MasterTimer()");
  }
}
