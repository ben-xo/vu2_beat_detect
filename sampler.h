/*
 * Copyright Ben XO https://github.com/ben-xo All rights reserved.
 */

#ifndef _SAMPLER_H
#define _SAMPLER_H

// sample buffer. this is written into by an interrupt handler serviced by the ADC interrupt.
extern byte samples[SAMP_BUFF_LEN];
extern volatile uint8_t current_sample;
extern volatile uint8_t new_sample_count;
void setup_sampler();

#endif /* _SAMPLER_H */
