#include <Arduino.h>
#include <DigitalIO.h>

#include "ledpwm.h"
#include "buttons.h"
#include "debug.h"

// these are inherited from the main cpp.
extern DigitalPin<BUTTON_PIN> button_pin;
extern DigitalPin<BEAT_PIN>   beat_pin;
extern DigitalPin<TEMPO_PIN>  tempo_pin;

void debug_loop() {
  static uint32_t last_time = 0;
  uint32_t now;
  static uint8_t mode = 255;

  portb_val = 0;

  while(true) {
    now = millis();
    if(now - last_time > 100) {
      mode++;
      last_time = now;

      switch(mode) {
        default:
        case 0:
          mode = 0;
          tempo_pin.low();
          portb_val = 1;
          break;
          
        case 1:
          portb_val = 3;
          break;
    
        case 2:
          portb_val = 7;
          break;
    
        case 3:
          portb_val = 15;
          break;
    
        case 4:
          portb_val = 0;
          beat_pin.high();
          break;

        case 5:
          beat_pin.low();
          tempo_pin.high();
          break;

      }
    }

    while(button_pin.read()) {
      delay(1);
    }    
  }
}
