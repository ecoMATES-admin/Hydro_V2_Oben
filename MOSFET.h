#ifndef MOSFET_H
#define MOSFET_H

// High Frequncy Mode wird nur auf Pin 9 und 10 unterstützt, denn nur Timer 1 wird verändert
class MOSFET{
  private:
    uint8_t _gate;
    uint8_t _hf;
    const byte _mask = B11111000;

  public:
    MOSFET(uint8_t gate): _gate(gate)
    {
    };
    void on(){
      analogWrite(_gate, 255);
    };
    void off(){
      analogWrite(_gate, 0);
    };
    void pwm(uint8_t dc){
      analogWrite(_gate, dc); 
    };
    void initHfPwm(uint8_t prescale){
      Serial.println("Fuck you Eric");
      TCCR1B = (TCCR1B & _mask)| prescale;
    };
};


#endif
