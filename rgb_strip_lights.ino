/*
 * Copyright (c) 2015 Nick Robinson All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0 which accompanies this
 * distribution, and is available at http://www.gnu.org/licenses/gpl.html
/*

/*
 * A controller for RGB LED strips.
 * Lights LEDs in funky patterns using PWM to choose the RGB values.
 * There are many "modes", each mode displays a different pattern of lights.
 * The simplest mode is static white
 * Then there are predefined patterns, such as "jump3", which show, red, green and then blue.
 * There are also used defined sequences, where you can choose which colours to display.
 *
 * See the "MODES" section below for the list of all the modes. 
 *
 * The speed of the pattern can be changed using a potentiometer (not yet implemented).
 *
 * USER GUIDE
 * ==========
 * 
 * Press the SEQUENCE button, to move to the next pattern of colours (will cycle back to the start when you reach the end)
 * Press the MODE button, to change mode (will cycle back to the start when you reach the end)
 *
 * Turn the speed dial at any time to change the rate of flashing. (not yet implemented)
 *
 * To change a colour within a sequence begin by pressing the "Edit" button.
 *   The LEDs will stop, and show you just a single colour within the sequence.
 *   Repeatedly press the "SEQUENCE" button until you get to the colour you want to change.
 *   Press "EDIT" again, to change this colour
 *     The LEDs will change to the colour specified by the RGB dials (the potentiometers)
 *     Turn the RGB dials till you get the desired colour
 *   Press "EDIT" again to save this colour.
 *   You can press the "MODE" button at any time to end editting.
 *
 * To add a new colour into a sequence begin by pressing the "Edit" button.
 *   The LEDs will stop, and show you just a single colour within the sequence.
 *   Repeatedly press the "SEQUENCE" button until you get to the place you want to add the colour.
 *   NOTE. The new colour will be added AFTER the colour you stop on.
 *   Press the "ADD" button.
 *     The LEDs will change to the colour specified by the RGB dials (the potentiometers)
 *     Turn the RGB dials till you get the desired colour
 *   Press "ADD" (or "EDIT") to save this new colour.
 *   You can press the "MODE" button at any time to end editting.
 * 
 * To delete a colour from a sequence, begin by pressing the "EDIT" button
 *   The LEDs will stop, and show you just a single colour within the sequence.
 *   Repeatedly press the "SEQUENCE" button until you get to the colour you want to delete
 *   Press the "DELETE" button
 *   The LEDs will flash RED and the colour that will be deleted.
 *   Press the "DELETE" button again to delete the colour.
 *   You can press the "MODE" button at any time to end editting.
 * 
 */
#include <EEPROM.h>
#include "debug.h"
#include "controller.h"

void setup()
{
    controller.setup();
}

void loop()
{
    controller.loop();
}

