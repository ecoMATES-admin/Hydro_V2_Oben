void FSM_Sensordata() {
  switch (sensorState) {
    case sensorStates::Idle:
      if (sampleFlagTop) {
        sensorState = sensorStates::ReadTop;
        sampleFlagTop = false;
      }
      break;
    case sensorStates::ReadTop:
      tempHumValTop[1] = shtTop.readTemperature();
      tempHumValTop[0] = shtTop.readHumidity();
      if (DEBUG) {
        Serial.print("Top: \t");
        Serial.print(tempHumValTop[0]);
        Serial.print("\t");
        Serial.println(tempHumValTop[1]);
      }
      sensorState = sensorStates::ReadOutside;
      break;
    case sensorStates::ReadOutside:
      tempHumValOutside[1] = shtOutside.readTemperature();
      tempHumValOutside[0] = shtOutside.readHumidity();
      if (DEBUG) {
        Serial.print("Top: \t");
        Serial.print(tempHumValOutside[0]);
        Serial.print("\t");
        Serial.println(tempHumValOutside[1]);
      }
      sensorState = sensorStates::Idle;
      break;
    default:
      Serial.println("Error: FSM_Sensordata default state");
  }
}
