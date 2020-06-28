/*
 * Copyright Ben XO https://github.com/ben-xo All rights reserved.
 */


#ifndef _DEBUG_H
#define _DEBUG_H

#include "config.h"

#ifdef DEBUG_SAMPLE_RATE
  #define DEBUG_SAMPLE_RATE_HIGH() (DEBUG_SAMPLE_RATE_PORT |= (1 << DEBUG_SAMPLE_RATE_PIN))
  #define DEBUG_SAMPLE_RATE_LOW()  (DEBUG_SAMPLE_RATE_PORT &= ~(1 << DEBUG_SAMPLE_RATE_PIN))
#else
  #define DEBUG_SAMPLE_RATE_HIGH() ()
  #define DEBUG_SAMPLE_RATE_LOW()  ()
#endif

#ifdef DEBUG_FRAME_RATE
  #define DEBUG_FRAME_RATE_HIGH() (DEBUG_FRAME_RATE_PORT |= (1 << DEBUG_FRAME_RATE_PIN))
  #define DEBUG_FRAME_RATE_LOW()  (DEBUG_FRAME_RATE_PORT &= ~(1 << DEBUG_FRAME_RATE_PIN))
#else
  #define DEBUG_FRAME_RATE_HIGH() ()
  #define DEBUG_FRAME_RATE_LOW()  ()
#endif


void debug_loop();

#endif /* _DEBUG_H */
