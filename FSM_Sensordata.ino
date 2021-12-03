void FSM_Sensordata() {
  switch (sensorState) {
    case sensorStates::Idle:
      if (sampleFlagTop) {
        sensorState = sensorStates::ReadTop;
        sampleFlagTop = false;
      }
      break;
    case sensorStates::ReadTop:
      if (!dhtSensorTop.readTempAndHumidity(tempHumValTop)) {
        if (DEBUG) {
          Serial.print("Top: \t");
          Serial.print(tempHumValTop[0]);
          Serial.print("\t");
          Serial.println(tempHumValTop[1]);
        }
      }else{
        //watchdog?
      }
      
        sensorState = sensorStates::ReadOutside;
      break;
    case sensorStates::ReadOutside:
      if (!dhtSensorOutside.readTempAndHumidity(tempHumValOutside)) {
        if (DEBUG) {
          Serial.print("Outside:");
          Serial.print(tempHumValOutside[0]);
          Serial.print("\t");
          Serial.println(tempHumValOutside[1]);
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
