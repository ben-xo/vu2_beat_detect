/*
 * Copyright Ben XO https://github.com/ben-xo All rights reserved.
 */

#ifndef _BUTTONS_H
#define _BUTTONS_H

#include <DigitalIO.h>
#include "config.h"

#define SHORT_PUSH 1
#define LONG_PUSH 2

uint8_t was_button_pressed(DigitalPin<BUTTON_PIN> &button);


#endif /* _BUTTONS_H */
