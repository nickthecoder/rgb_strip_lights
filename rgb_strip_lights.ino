/*
 * Copyright (c) 2015 Nick Robinson All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0 which accompanies this
 * distribution, and is available at http://www.gnu.org/licenses/gpl.html
/*

/*
 * A controller for RGB LED strips.
 * Lights LEDs in funky patterns using PWM to choose the RGB values.
 * There are many "modes", each mode displays a different pattern of lights.
 *
 * See the "MODES" section below for the list of all the modes. 
 *
 * USER GUIDE
 * ==========
 * 
 * Press the SEQUENCE button, to move to the next pattern of colours (will cycle back to the start when you reach the end)
 * Press the MODE button, to change mode (will cycle back to the start when you reach the end)
 *
 * Turn the speed dial at any time to change the rate of flashing.
 *
 * To change a colour within a sequence begin by pressing the "Edit" button.
 *   The LEDs will stop, and show you just a single colour within the sequence.
 *   Repeatedly press the "SEQUENCE" button until you get to the colour you want to change.
 *   Press "EDIT" again, to change this colour
 *     The LEDs will change to the colour specified by the RGB dials (the potentiometers)
 *     Turn the RGB dials till you get the desired colour
 *   Press "EDIT" again to save this colour.
 *   You can press the "MODE" button at any time to end editing.
 *
 * To add a new colour into a sequence begin by pressing the "Edit" button.
 *   The LEDs will stop, and show you just a single colour within the sequence.
 *   Repeatedly press the "SEQUENCE" button until you get to the place you want to add the colour.
 *   NOTE. The new colour will be added AFTER the colour you stop on.
 *   Press the "ADD" button.
 *     The LEDs will change to the colour specified by the RGB dials (the potentiometers)
 *     Turn the RGB dials till you get the desired colour
 *   Press "ADD" to save this new colour.
 *   You can press the "MODE" button at any time to end editting.
 * 
 * To delete a colour from a sequence, begin by pressing the "EDIT" button
 *   The LEDs will stop, and show you just a single colour within the sequence.
 *   Repeatedly press the "SEQUENCE" button until you get to the colour you want to delete
 *   Press the "DELETE" button
 *   The LEDs will flash RED and the colour that will be deleted.
 *   Press the "DELETE" button again to delete the colour.
 *   Or press the "MODE" button to cancel.
 *
 * To add a new SEQUENCE of colours, begin by pressing the "ADD" button
 *    The LEDs will stop, and show you just a single colour.
 *    Turn the RGB dials till you get the desired colour
 *    Press the "ADD" button again, or "MODE" to cancel
 *     
 *    You have now created a sequence containg just a single colour.
 *    If you want to add more colours to the sequence, follow the instructions for adding colours (see above)
 *    
 * To delete a SEQUENCE, begin by pressing the "DELETE" button.
 *   The lights will flash crazy, mostly red, but also all the colours in the sequence you are about to delete
 *   Press the "DELETE" button again to confirm,
 *   Or press "MODE" button to cancel.
 *
 * Remote Control Keys :
 * DIY1 : Edit
 * DIY2 : Add
 * DIY3 : Delete
 * FLASH: Next Mode
 * Jump3: Previous Sequence
 * Jump7: Next Sequence
 * Fade3: Previous Ease
 * Fade7: Next Ease
 * POWER: Low light/Reset/Cancel
 
 * The following buttons are not currently used
 
 * AUTO:     (could be a demo mode, which picks random settings every now and then)
 * DIY4 :    (could be twinkle down)
 * DIY5 :    (could be twinkle up)
 * DIY6 :    (could be previous mode)
 * Colour Up/Down arrows: (could Choose custom colours instead of using the dials)
 *
 * All of the other buttons should be obbvious.
 */

#include <U8glib.h>

#include <EEPROM.h>
#include "debug.h"
#include "controller.h"

void setup()
{
    Serial.begin( 9600 );
    Serial.println( "Setup" );  

    controller.setup();

    dprintln( "setting up remote" );
    controller.remote.setup();
    dprintln( "set up remote" );
}

void loop()
{
    controller.loop();
}

/*
Planned Improvements

previousMode isn't on remote control yet. DIY6 ? (left of flash).
  
Add twinkle speed on remote control DIY4/5 = up/down.

Demo mode - picks a random sequence, mode and ease every N seconds.
  "Auto" button on the remote.

StayMode detect waggle on RGB dials and switch to StaticMode

*/

