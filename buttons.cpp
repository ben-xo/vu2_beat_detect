/*
 * Copyright Ben XO https://github.com/ben-xo All rights reserved.
 */

#include <Arduino.h>
#include <DigitalIO.h>
#include "buttons.h"

// TODO: is it worth optimising this to user only lower uitn16_t of millis() as the duration we're measuring is short?

// returns true on the falling edge of a button push
uint8_t was_button_pressed(DigitalPin<BUTTON_PIN> &button) {
  static bool was_down = false;
  static uint32_t last_push;

  bool is_down = button.read();
  
  if(was_down && !is_down) {
    was_down = false;
    if(millis() - last_push > 3000) {
      // long push
      return LONG_PUSH;
    }
    // short push
    return SHORT_PUSH;
  }
  if(!was_down && is_down) {
    was_down = true;
    last_push = millis();
  }
  // no push yet, although one may be in progress.
  return 0;
}
