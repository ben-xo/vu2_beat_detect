#include <Arduino.h>
#include "config.h"
#include "sampler.h"

#include "PeckettIIRFixedPoint.h"

static float thresh_f = 0.8f;

// Our Global Sample Rate, 5000hz
#define SAMPLEPERIODUS 200

// 20 - 200hz Single Pole Bandpass IIR Filter
static float bassFilter(float sample) {
    static float xv[3] = {0,0,0}, yv[3] = {0,0,0};
    xv[0] = xv[1]; xv[1] = xv[2]; 
    xv[2] = sample / 9.1f;
    yv[0] = yv[1]; yv[1] = yv[2]; 
    yv[2] = (xv[2] - xv[0])
        + (-0.7960060012f * yv[0]) + (1.7903124146f * yv[1]);
    return yv[2];
}

// 10hz Single Pole Lowpass IIR Filter
static float envelopeFilter(float sample) { //10hz low pass
    static float xv[2] = {0,0}, yv[2] = {0,0};
    xv[0] = xv[1]; 
    xv[1] = sample / 160.f;
    yv[0] = yv[1]; 
    yv[1] = (xv[0] + xv[1]) + (0.9875119299f * yv[0]);
    return yv[1];
}

// 1.7 - 3.0hz Single Pole Bandpass IIR Filter
static float beatFilter(float sample) {
    static float xv[3] = {0,0,0}, yv[3] = {0,0,0};
    xv[0] = xv[1]; xv[1] = xv[2]; 
    xv[2] = sample / 7.015f;
    yv[0] = yv[1]; yv[1] = yv[2]; 
    yv[2] = (xv[2] - xv[0])
        + (-0.7169861741f * yv[0]) + (1.4453653501f * yv[1]);
    return yv[2];
}


void PeckettIIRSetup() {
  setup_sampler(SAMPLER_TIMER_COUNTER_FOR(5000 /* Hz */));
}

void PeckettIIR(uint16_t val, DigitalPin<BEAT_PIN> beat_pin) {
    float sample, value, envelope, beat;
//    static boolean is_beat;
    static uint8_t i = 0;
    
    // Read ADC and center so +-512
    sample = (float)val-120.f;

    // Filter only bass component
    value = bassFilter(sample);

    // Take signal amplitude and filter
    if(value < 0)value=-value;
    envelope = envelopeFilter(value);

    // Every 200 samples (25hz) filter the envelope 
    if(i == 200) {
      // Filter out repeating bass sounds 100 - 180bpm
      beat = beatFilter(envelope);
//Serial.println(beat);

      // If we are above threshold, light up LED
      if (beat > thresh_f) {
        beat_pin.high();
      } else {
        beat_pin.low();
      }
      
      //Reset sample counter
      i = 0;
    } else {
      i++;
    }
}
