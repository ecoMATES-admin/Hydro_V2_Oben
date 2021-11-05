void FSM_Sensordata() {
  switch (sensorState) {
    case sensorStates::Idle:
      if (sampleFlagTop) {
        sensorState = sensorStates::ReadTop;
        sampleFlagTop = false;
      }
      break;
    case sensorStates::ReadTop:
      if (!dhtSensorTop.readTempAndHumidity(tempHumTop)) {
        if (DEBUG) {
          Serial.print("Top: \t");
          Serial.print(tempHumTop[0]);
          Serial.print("\t");
          Serial.println(tempHumTop[1]);
        }
      }else{
        //watchdog?
      }
      
        sensorState = sensorStates::ReadOutside;
      break;
    case sensorStates::ReadOutside:
      if (!dhtSensorOutside.readTempAndHumidity(tempHumOutside)) {
        if (DEBUG) {
          Serial.print("Outside:");
          Serial.print(tempHumOutside[0]);
          Serial.print("\t");
          Serial.println(tempHumOutside[1]);
        }
      }else{
        //watchdog?
      }
      sensorState = sensorStates::Idle;
      break;
    default:
      Serial.println("Error: FSM_Sensordata default state");
  }
}
