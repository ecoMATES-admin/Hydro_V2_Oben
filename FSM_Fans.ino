void FSM_CirculationFan() {
  switch (circulationFanState) {
    case circulationFanStates::FanOn:
      fanCirculation.on();
      if (!circulationFanOnFlag) {
        circulationFanState = circulationFanStates::FanOff;
      }
      break;
    case circulationFanStates::FanOff:
      fanCirculation.off();
      if (circulationFanOnFlag) {
        circulationFanState = circulationFanStates::FanOn;
      }
      break;
      default:
      Serial.println("Error: FSM_CirculationFan()");
  }
}

void FSM_FilterFan() {
  switch (filterFanState) {
    case filterFanStates::FanOn:
      fanFilter.on();
      break;
      default:
      Serial.println("Error: FSM_FilterFan()");
  }
}

void FSM_LedsFan() {
  switch (ledsFanState) {
    case ledsFanStates::FanOn:
      fanLeds.on();
      if (!ledsFanOnFlag) {
        ledsFanState = ledsFanStates::FanOff;
      }
      break;
    case ledsFanStates::FanOff:
      fanLeds.off();
      if (ledsFanOnFlag) {
        ledsFanState = ledsFanStates::FanOn;
      }
      break;
      default:
      Serial.println("Error: FSM_LedsFan()");
  }
}
