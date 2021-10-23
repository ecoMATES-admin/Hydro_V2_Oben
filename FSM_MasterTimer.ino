void FSM_MasterTimer(){
  switch(masterTimerState){
    case masterTimerStates::CheckTime:
    break;
    case masterTimerStates::cmdLight:
    break;
    case masterTimerStates::cmdPump:
    break;
    case masterTimerStates::cmdSensors:
    break;
    default:
    Serial.println("Error: FSM_MasterTimer()");
  }
}
