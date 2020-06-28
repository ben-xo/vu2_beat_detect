Beat Detection to go with vu2
=============================

This is a self indulgent project by Ben XO https://github.com/ben-xo

The idea is that you feed line level audio into a pin on an arduino, and you get a flashing LED when the beat happens. Wire it to another Arduino running vu2 (from my GitHub) to get extra effects.

License
-------

Copyright Ben XO https://github.com/ben-xo All rights reserved.

THIS PROJECT IS NOT OPEN SOURCE. Whilst I have put the source code on GitHub, no part of this project is currently released under any open source license.

I reserve all rights and the right to change this license in future.

What does it look like?
=======================

TODO

Features
========

* Works from line-in
* WORK IN PROGRESS
* Has two modes: Damian Peckett's beat detection algorithm and a Fixed Point version which is not as good but uses far less CPU

How to use
==========

The basic hardware is simple:

* Arduino
* Audio into A0 via a small capacitor and a potential divider. See the diagram on Damian Peckett's blog
* 4 LEDs on D8, D9, D10 and D11 (no need for resistors - there's a fast interrupt driven PWM routine)
* button on D4, pulled low with a 10k
* Oscilloscope taps on A1 and A2 to measure the sample rate (5kHz) and the duty cycle of the beat detect routine.

Software
--------

1) Set up Arduino Studio
2) Install DigitalIO lib
3) Load into Arduino Studio
4) program the arduino over USB.
5) if you hold down the button during startup, it will cycle through all the LEDs so you know they're connected properly.

Finally
-------

Plug the audio in. Plug the power in. Adjust the volume...

Push button to change modes. (Mode 0 is Damian Peckett's, mode 1 is the Fixed Point version. There's room to add more algorithms to try them out!)

Tips
====

Mac owners can make open Audio MIDI Setup to create a virtual soundcard that combines both the speakers AND the line-out, so that you can play audio out loud AND into the lights simultaneously. Neat trick

Want to work on it?
-------------------

Yeah! You do! I have no idea what to tell you, ask me questions on Twitter @benxo I have loads of ideas and none of them are important.

Thanks for coming to my TED talk






