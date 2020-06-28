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

#include "PeckettIIR.h"
#include "PeckettIIRFixedPoint.h"

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

   Serial.begin(2000000);
}

void loop() {
  // jump to debug if the button is help down during startup.
  if(button_pin.read()) {
    setup_sampler(SAMPLER_TIMER_COUNTER_FOR(2500 /* Hz */));
    debug_loop();
  }

  algorithm_loop();  
}

/*****/

typedef void (*BeatDetectSetupList[])();
typedef void (*BeatDetectList[])(uint16_t val, DigitalPin<BEAT_PIN> beat_pin);
BeatDetectSetupList gBeatDetectAlgorithmSetup = { PeckettIIRSetup, PeckettIIRFixedPointSetup };
BeatDetectList gBeatDetectAlgorithm = { PeckettIIR, PeckettIIRFixedPoint };
uint8_t current_mode = 0;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

uint8_t next_algorithm()
{
  // add one to the current pattern number, and wrap around at the end
  return current_mode = (current_mode + 1) % ARRAY_SIZE( gBeatDetectAlgorithm );
}


void algorithm_loop() {
  uint8_t mode = 0;

  gBeatDetectAlgorithmSetup[mode](); // gotta start somewhere
  
  while(true) {

    if(was_button_pressed(button_pin)) {
      next_algorithm();
    }
  
    if(mode != current_mode) {
      mode = current_mode;
      portb_val = mode;
      gBeatDetectAlgorithmSetup[mode]();
    }

    if(new_sample_count) {
        cli();
        uint8_t sample_ptr = current_sample;
        new_sample_count--;
        uint8_t val = samples[sample_ptr];
        sei();

//        Serial.println(val);
        
        DEBUG_FRAME_RATE_HIGH();
        gBeatDetectAlgorithm[mode](val, beat_pin);
        DEBUG_FRAME_RATE_LOW();
    }
    
  }
}
