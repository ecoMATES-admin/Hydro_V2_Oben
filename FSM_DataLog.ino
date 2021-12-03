
void FSM_DataLog() {
  switch (dataLogState) {
    case dataLogStates::Idle:
      if (sendAllFlag >= 5) {
        sendAllFlag = 0;
        dataLogState = dataLogStates::SendAll;
        break;
      }
      if (sendWaterFlag) {
        sendWaterFlag = false;
        dataLogState = dataLogStates::SendWater;
      }
      break;
    case dataLogStates::SendAll:
      if(DEBUG)
        Serial.println("dataLogStates::SendAll");
      dtostrf(tempHumValBottom[0], 4, 2, dataBuffer);
      NodeSerial.print("VXa");
      NodeSerial.println(dataBuffer);
      memset(dataBuffer, 0, 6);
      dtostrf(tempHumValBottom[1], 4, 2, dataBuffer);
      NodeSerial.print("b");
      NodeSerial.println(dataBuffer);
      memset(dataBuffer, 0, 6);
      dtostrf(tempHumValTop[0], 4, 2, dataBuffer);
      NodeSerial.print("c");
      NodeSerial.println(dataBuffer);
      memset(dataBuffer, 0, 6);
      dtostrf(tempHumValTop[1], 4, 2, dataBuffer);
      NodeSerial.print("d");
      NodeSerial.println(dataBuffer);
      memset(dataBuffer, 0, 6);
      dtostrf(tempHumValOutside[0], 4, 2, dataBuffer);
      NodeSerial.print("e");
      NodeSerial.println(dataBuffer);
      memset(dataBuffer, 0, 6);
      dtostrf(tempHumValOutside[1], 4, 2, dataBuffer);
      NodeSerial.print("f");
      NodeSerial.println(dataBuffer);
      memset(dataBuffer, 0, 6);
      dtostrf(waterTempVal, 4, 2, dataBuffer);
      NodeSerial.print("g");
      NodeSerial.println(dataBuffer);
      memset(dataBuffer, 0, 6);
      dtostrf(phVal, 4, 2, dataBuffer);
      NodeSerial.print("h");
      NodeSerial.println(dataBuffer);
      memset(dataBuffer, 0, 6);
      dtostrf(ecVal, 4, 2, dataBuffer);
      NodeSerial.print("i");
      NodeSerial.println(dataBuffer);
      memset(dataBuffer, 0, 6);
      
      dataLogState = dataLogStates::Idle;
      break;
    case dataLogStates::SendWater:
      if(DEBUG)
        Serial.println("dataLogStates::SendWater");
      dtostrf(waterLevelVal, 4, 2, dataBuffer);
      NodeSerial.print("VYw");
      NodeSerial.println(dataBuffer);
      memset(dataBuffer, 0, 6);
      
      dataLogState = dataLogStates::Idle;
      break;
    default:
      Serial.println("Error: FSM_DataLog()");
  }
}
