/*
 * Copyright Ben XO https://github.com/ben-xo All rights reserved.
 */

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "config.h"
#include "sampler.h"
#include "debug.h"

// sample buffer. this is written into by an interrupt handler serviced by the ADC interrupt.
byte samples[SAMP_BUFF_LEN] __attribute__((__aligned__(256))); // TODO: not sure if this alignment is useful
volatile uint8_t current_sample = 0;
volatile uint8_t new_sample_count = 0;

/**
 * timer_counter = (F_CPU / (1 * desired_sample_frequency) - 1)
 * 
 * that "1" is the prescaler
 */
void setup_sampler(uint16_t timer_counter) {

  cli();
  ADCSRA = 0;             // clear ADCSRA register
  ADCSRB = 0;             // clear ADCSRB register
  ADMUX |= (AUDIO_INPUT_PIN & 0x07)     // set A0 analog input pin
        |  (1 << REFS0)   // set reference voltage to internal 1.1v (gives a signal boost for audio).
        |  (1 << REFS1)   // set reference voltage to internal 1.1v (gives a signal boost for audio).
        |  (1 << ADLAR)   // left align ADC value to 8 bits from ADCH register
  ;

  // sampling rate is [ADC clock] / [prescaler] / [conversion clock cycles]
  // for Arduino Uno ADC clock is 16 MHz and a conversion takes 13 clock cycles

  ADCSRA  = 0
//         | (1 << ADPS2) 
         | (1 << ADPS1) 
         | (1 << ADPS0)
  ; // 128 prescaler for 9600 Hz, which is the slowest it will do.

//ADCSRA |= (1 << ADATE) // enable auto trigger
//ADCSRA |= (1 << ADIE)  // enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN)  // enable ADC
//       |  (1 << ADSC)  // start ADC measurements
  ;

  // TIMER 1 for interrupt frequency 5000 Hz:  
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 109; // initialize counter value to 0
  // set compare match register correctly (passed in)
  OCR1A = timer_counter;
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12, CS11 and CS10 bits for 1 prescaler
  TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

#ifdef DEBUG_SAMPLE_RATE
  // debugging pin for checking sample rate
  pinMode (DEBUG_SAMPLE_RATE_PIN, OUTPUT);
#endif
  
  sei();
}

ISR(TIMER1_COMPA_vect)
{
  DEBUG_SAMPLE_RATE_HIGH();

  ADCSRA |= (1 << ADSC); // trigger next analog sample.
  
  uint8_t sample_idx = (current_sample + 1) & SAMP_OFFSET_MASK;
  current_sample = sample_idx;
  
  byte sample = ADCH;
  volatile byte* the_sample = samples + current_sample;
  *the_sample = sample;
  new_sample_count++;

  DEBUG_SAMPLE_RATE_LOW();
}
