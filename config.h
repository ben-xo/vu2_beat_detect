#ifndef _CONFIG_H
#define _CONFIG_H

/*** Pin configuration ***/
#define AUDIO_INPUT_PIN 0
#define TEMPO_PIN 2
#define BEAT_PIN 3
#define BUTTON_PIN 4
#define MODE_LED_PIN_1 8
#define MODE_LED_PIN_2 9
#define MODE_LED_PIN_3 10 
#define MODE_LED_PIN_4 11

/*** LED PWM configuration ***/
#define PWM_LED_FRQ      10000 // 10kHz
#define PWM_DUTY_PERCENT 10

/*** Audio sampling config ***/
#define SAMP_BUFF_LEN 256 // this needs to be a power of 2
#define SAMP_OFFSET_MASK (SAMP_BUFF_LEN - 1)

/*** random debug stuff ***/
#define DEBUG_SAMPLE_RATE 1
#define DEBUG_SAMPLE_RATE_PORT PORTC
#define DEBUG_SAMPLE_RATE_PIN PC1
#define DEBUG_FRAME_RATE 1
#define DEBUG_FRAME_RATE_PORT PORTC
#define DEBUG_FRAME_RATE_PIN PC2

#endif /* _CONFIG_H */
