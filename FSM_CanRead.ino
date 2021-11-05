void FSM_CanRead() {

  instruction tempInst; // create temporary instruction to store received can message
  instructionFloat tempInstF;
  float tempF;

  if (canBuffer.read(tempInst)) {
    switch (tempInst.data[1]) {
      case 0:
      Serial.println("Sensor command:");
        break;
      case 1:
      Serial.println("Pump command:");
        break;
      case 2:
      Serial.print("Light command:");
      Serial.println(tempInst.data[0]);
        break;
      case 9:
        break;
      case 10:
        break;
      case 11:
        break;
      case 12:
        break;
    }
  }

  if (canFloatBuffer.read(tempInstF)) {
    tempF = canToFloat(tempInstF.data[5], tempInstF.data[4], tempInstF.data[0]);
    switch (tempInstF.data[1]) {
      case 3:
        tempHumValBottom[1]= tempF;
        if(DEBUG){
        Serial.print("Temp Bottom: ");
        Serial.println(tempHumValBottom[1]);
        }
        break;
      case 4:
      tempHumValBottom[0]= tempF;
        if(DEBUG){
        Serial.print("Hum Bottom: ");
        Serial.println(tempHumValBottom[0]);
        }
        break;
      case 5:
      waterTempVal= tempF;
        if(DEBUG){
        Serial.print("Watertemp: ");
        Serial.println(waterTempVal);
        }
        break;
      case 6:
      ecVal= tempF;
        if(DEBUG){
        Serial.print("EC: ");
        Serial.println(ecVal);
        }
        break;
      case 7:
      phVal= tempF;
        if(DEBUG){
        Serial.print("PH: ");
        Serial.println(phVal);
        }
        break;
      case 8:
      waterLevelVal= tempF;
        if(DEBUG){
        Serial.print("Waterlevel: ");
        Serial.println(waterLevelVal);
        }
        break;
    }
  }
}

void canWrite(uint8_t purpose, uint8_t data = 0, uint16_t floatData = 65535) {
  my_can_msg canMsg;
  uint32_t id = 0;
  canMsg.bytes[3] = 0;
  canMsg.bytes[2] = 1;
  canMsg.bytes[1] = purpose;
  canMsg.bytes[0] = data;
   Serial.print("MCP Error:");
   Serial.println(write(id, canMsg.value, floatData));
}

MCP2515::ERROR write(uint32_t id, uint32_t val, uint16_t floatData) {
  Serial.println("canWrite");
  can_frame frame;
  frame.can_id = id;
  my_can_msg msg;
  msg.value = val;

  if (floatData == 65535) {
    frame.can_dlc = 4;
    for ( int8_t i = 0; i < 4; i++ ) {
      frame.data[i] = msg.bytes[i];
      Serial.println(msg.bytes[i]);
    }
  } else {
    frame.can_dlc = 6;
    for ( int i = 0; i < 4; i++ ) { //prepare can message
      frame.data[i] = msg.bytes[i];
      Serial.println(msg.bytes[i]);
    }
    frame.data[4] = splitInt(floatData, LSB);
    Serial.println(frame.data[4]);
    frame.data[5] = splitInt(floatData, MSB);
    Serial.println(frame.data[5]);
  }
  return mcp2515.sendMessage(&frame);
}

void canRead() { //!!!!Check excecution time -> Read out one message at a time?!!!!
  interrupt = false;
  if ( mcp2515_overflow ) {
    Serial.println( "\tMCP2516 RX Buf Overflow" );
    mcp2515_overflow = false;
  }
  if ( arduino_overflow ) {
    Serial.println( "\tArduino Buffers Overflow" );
    arduino_overflow = false;
  }
  can_frame frame; //Can Message Temp
  instruction instr; // FSM Instruction Temp
  instructionFloat instrF; // Float Instruction Temp
  bool hasData;
  cli(); hasData = cfStream.get( frame ); sei();
  while (hasData) {
    if (DEBUG)
      Serial.println("read");
    if (frame.can_dlc == 4) {
      for (int i = 0; i < 3; i++) { // Is there a better way, faster, and no copying values?
        instr.data[i] = frame.data[i];
        Serial.println(instr.data[i]);
      }
    } else if (frame.can_dlc == 6) {
      for (int i = 0; i < 6; i++) { // Is there a better way, faster, and no copying values?
        instrF.data[i] = frame.data[i];
        Serial.println(instrF.data[i]);
      }
    } else {
      Serial.println("Error in Can read()");
    }

    switch (frame.data[1]) { //check which FSM needs to hear about this can message -> store in respective vector
      case 0://wake-up
      case 1:
      case 2:
        if (!(canBuffer.write(instr)))
          Serial.println("canBuffer overflow");
        break;
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
        if (!(canFloatBuffer.write(instrF)))
          Serial.println("canFloatBuffer overflow");
        break;
      case 9:
      case 10:
      case 11:
      case 12:
        if (!(canBuffer.write(instr)))
          Serial.println("canBuffer overflow");
        break;
    }
    cli(); hasData = cfStream.get( frame ); sei();
  }
}
void irqHandler() {
  can_frame frm;
  uint8_t irq = mcp2515.getInterrupts();
  //check messages in buffer 0
  if ( irq & MCP2515::CANINTF_RX0IF ) {
    mcp2515.readMessage( MCP2515::RXB0, & frm );
    if ( !cfStream.put( frm ) ) //no space
      arduino_overflow = true;
  }

  //check messages in buffer 1
  if ( irq & MCP2515::CANINTF_RX1IF ) {
    mcp2515.readMessage( MCP2515::RXB1, & frm);
    if ( !cfStream.put( frm ) ) //no space
      arduino_overflow = true;
  }
  irq = mcp2515.getErrorFlags(); //read EFLG
  if ( (irq & MCP2515::EFLG_RX0OVR) | (irq & MCP2515::EFLG_RX1OVR) ) {
    mcp2515_overflow = true;
    mcp2515.clearRXnOVRFlags();
  }
  mcp2515.clearInterrupts();
  interrupt = true;
}

uint8_t getExponent(float tempF) {
  if (static_cast<long>(tempF * 100) < 65535) {
    return 100;
  } else {
    return 10;
  }
}

uint8_t splitInt(uint16_t floatData, uint8_t byt) {
  if ( byt == LSB) {
    return (floatData & 0xFF);
  } else if (byt == MSB) {
    return ((floatData >> 8) & 0xFF);
  } else {                                          //Just for testing purposes
    Serial.println("splitInt() wrong byt input!!!!");
    return 0;
  }
}

uint16_t floatToCan(float f, uint8_t e) {
  return static_cast<uint16_t>(f * e);
}

float canToFloat(uint8_t msb, uint8_t lsb, uint8_t e) {
  uint16_t tempF = (msb << 8) | lsb;
  return (static_cast<float>(tempF)) / e;
}
