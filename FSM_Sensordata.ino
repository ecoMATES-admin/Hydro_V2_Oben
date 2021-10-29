void FSM_Sensordata() {
  switch (sensorState) {
    case sensorStates::Idle:
      if (sampleFlagTop) {
        sensorState = sensorStates::ReadValues;
        sampleFlagTop = false;
      }
      break;
    case sensorStates::ReadValues:
      readCounter++;
      if (!dhtSensorTop.readTempAndHumidity(tempHumTop)) {
        dthTopCounter++;
        tempHumTopMean[0] += tempHumTop[0];
        tempHumTopMean[1] += tempHumTop[1];
        if (DEBUG) {
          Serial.print("Top: \t");
          Serial.print(tempHumTop[0]);
          Serial.print("\t");
          Serial.println(tempHumTop[1]);
        }
      }
      if (!dhtSensorOutside.readTempAndHumidity(tempHumOutside)) {
        dthOutsideCounter++;
        tempHumOutsideMean[0] += tempHumOutside[0];
        tempHumOutsideMean[1] += tempHumOutside[1];
        if (DEBUG) {
          Serial.print("Outside:");
          Serial.print(tempHumOutside[0]);
          Serial.print("\t");
          Serial.println(tempHumOutside[1]);
        }
      }
      if (readCounter >= n)
        sensorState = sensorStates::CalculateMean;
      break;
    case sensorStates::CalculateMean:
      if (dthTopCounter == 0) {
        tempHumValTop[0] = humError;
        tempHumValTop[1] = tempError;
      } else {
        tempHumValTop[0] = tempHumTopMean[0] / float(dthTopCounter) ;
        tempHumValTop[0] = tempHumTopMean[0] / float(dthTopCounter) ;
      }
      if (DEBUG) {
        Serial.print("Top mean:");
        Serial.print(tempHumValTop[0]);
        Serial.print("\t");
        Serial.println(tempHumValTop[1]);
      }
      if (dthOutsideCounter == 0) {
        tempHumValOutside[0] = humError;
        tempHumValOutside[1] = tempError;
      } else {
        tempHumValOutside[0] = tempHumOutsideMean[0] / float(dthOutsideCounter) ;
        tempHumValOutside[0] = tempHumOutsideMean[0] / float(dthOutsideCounter) ;
      }
      if (DEBUG) {
        Serial.print("Out. mean: \t");
        Serial.print(tempHumValOutside[0]);
        Serial.print("\t");
        Serial.println(tempHumValOutside[1]);
      }
      readCounter = 0; //timer for ReadValues State
      dthTopCounter = 0;
      dthOutsideCounter = 0;
      sensorState = sensorStates::Idle;
      break;
    default:
      Serial.println("Error: FSM_Sensordata default state");
  }
}
