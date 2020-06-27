/*
 * Copyright Ben XO https://github.com/ben-xo All rights reserved.
 */

#include <DigitalIO.h>

#include "config.h"

#include "ledpwm.h"
#include "sampler.h"
//#include "beatdetect.h"

#include "buttons.h"
#include "debug.h"

DigitalPin<BUTTON_PIN> button_pin;

DigitalPin<TEMPO_PIN>  tempo_pin;
DigitalPin<BEAT_PIN>   beat_pin;


void setup() {
  setup_ledpwm();  // timer 2 is entirely used by ledpwm. Also sets output pins to output
  
  button_pin.config(INPUT, LOW);
  
  beat_pin.config(OUTPUT, LOW);
  tempo_pin.config(OUTPUT, LOW);


#ifdef DEBUG_FRAME_RATE
  pinMode (DEBUG_FRAME_RATE_PIN, OUTPUT);
#endif  

  //setup_sampler((F_CPU / (1 * desired_sample_frequency) - 1));
}

void loop() {
  // jump to debug if the button is help down during startup.
  if(button_pin.read()) {
    setup_sampler(SAMPLER_TIMER_COUNTER_FOR(2500 /* Hz */));
    debug_loop();
  }

  
}
