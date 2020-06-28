/*
 * Copyright Ben XO https://github.com/ben-xo All rights reserved.
 */
 
#include <Arduino.h>
#include "config.h"
#include "sampler.h"

#include "PeckettIIR.h"

static int16_t thresh_f = 35;

// Our Global Sample Rate, 5000hz
#define SAMPLEPERIODUS 200

// 20 - 200hz Single Pole Bandpass IIR Filter
int16_t bassFilter(int16_t sample) {
    const int32_t Q = 24; // sample is only 10 bit precision, so we scale up for a bit more precision
    const int32_t alpha1 = (0.7960060012f * (1L << Q));
    const int32_t alpha2 = (1.7903124146f * (1L << Q));
  
    static int16_t xv[3] = {0L,0L,0L};
    static int32_t yv[3] = {0L,0L,0L};
    xv[0] = xv[1];
    xv[1] = xv[2]; 
    xv[2] = sample;
  
    yv[0] = yv[1]; 
    yv[1] = yv[2]; 

    int32_t x2_minus_x0 = ((int32_t)xv[2] - (int32_t)xv[0]) << Q;
    
    yv[2] = x2_minus_x0
            + ((-alpha1 * (yv[0]))>>Q) 
            + (( alpha2 * (yv[1]))>>Q);

    // manually convert from 32-bit fixed-point to 16-bit int
    // (this generates assembler about 2x as fast as the right-rotate alone)

    return (yv[2] >> (Q)); // doesn't quite match Q, but we'll take the scaling!
    
    // return ret;
}

// 10hz Single Pole Lowpass IIR Filter
int16_t envelopeFilter(int16_t sample) { //10hz low pass
    const int32_t Qinput = 4;
    const int32_t Qprocess = 16;
    const int32_t K = (1 << (Qprocess - 1)); // for rounding
    const int32_t alpha3 = (0.9875119299f * (1L << Qprocess));

    static int16_t xv[2] = {0,0};
    static int32_t yv[2] = {0,0};
    xv[0] = xv[1];


    // This should be roughly the same as "divide by 48" which is close to the 50 in the original
    // (plus the difference in point precision from input to processing)
    // but we're also doing the fixed point scaling at the same time, so in fact it's
    // << 16 (scale) and then >> 6 (divide by 64) and then >> 4 (input was scaled differently)
    // XXX: ... I've just done no scaling and it SEEMS TO BE OKAY
    xv[1] = sample;
    
    yv[0] = yv[1];
    
    int32_t x0_plus_x1 =  ((int32_t)xv[0] + (int32_t)xv[1]);
    
    yv[1] = x0_plus_x1
           + ((alpha3 * yv[0]) >> Qprocess);

    // manually convert from 32-bit fixed-point to 16-bit int
    // (this generates assembler about 2x as fast as the right-rotate alone)
    
    // XXX: this 11 was found by tuning. Need to derive it...
    // return yv[1] >> 11;
    
    int16_t top =           yv[1] >> 2;
    int16_t bot = (int16_t) yv[1];
    int16_t ret = (top << 1) + (bot >> 15);
    return top;
}

// 1.7 - 3.0hz Single Pole Bandpass IIR Filter
int16_t beatFilter(int16_t sample) {
    const int32_t Q = 16;
    const int32_t K = (1 << (Q - 1)); // for rounding
    const int32_t scaler = (int32_t)((1.0f / 2.7f)  * (1L << Q));
    const int32_t alpha4 = (int32_t)(-0.7169861741f * (1L << Q));
    const int32_t alpha5 = (int32_t)(1.4453653501f  * (1L << Q));
    
    static int16_t xv[3] = {0L,0L,0L};
    static int32_t yv[3] = {0L,0L,0L};
    
    xv[0] = xv[1]; 
    xv[1] = xv[2]; 
    xv[2] = sample;
    
    yv[0] = yv[1]; 
    yv[1] = yv[2];

    int32_t x2_minus_x0 = (int32_t) (xv[2] - xv[0]) * scaler; // equiv to << Q and /2.7

    yv[2] = x2_minus_x0
        + ((alpha4 * yv[0]) >> Q) 
        + ((alpha5 * yv[1]) >> Q);

    // manually convert from 32-bit fixed-point to 16-bit int
    // (this generates assembler about 2x as fast as the right-rotate alone)
    //  return yv[2] >> (Q-2);

    int16_t top =           yv[2] >> 16;
    int16_t bot = (int16_t) yv[2];
    int16_t ret = (top << 2) + (bot >> (Q-2));
    return top;
}

void PeckettIIRFixedPointSetup() {
  setup_sampler(SAMPLER_TIMER_COUNTER_FOR(5000 /* Hz */));
}

void PeckettIIRFixedPoint(uint16_t val, DigitalPin<BEAT_PIN> beat_pin) {
    int16_t sample, value, envelope, beat;
//    static boolean is_beat;
    static uint8_t i = 0;
    
    // Read ADC and center so +-512
    sample = val-120;
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
