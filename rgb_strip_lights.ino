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
 
// Increment this each time I change the program, vital if the EEPROM data structures change!
#define PROGRAM_VERSION 3

#include <EEPROM.h>

// **** PIN LAYOUT ****
int LED_PINS[3] = {9,10,11};            // PWM pins used as output for the LEDs
int RGB_IN_PINS[3] = {A0, A1, A2};      // Input pins for the RGB potentiometers
int BUTTON_PINS[5] = { 3, 4, 5, 6, 7 }; // Translates from a button index (zero based) to the pin for that button number.
#define SPEED_PIN A3                     // A pot which governs the how quickly to move through the colour sequence
#define EDIT_LED_PIN 8

// **** BUTTONS ****
#define BUTTON_COUNT 5 // The total number of buttons which use the readButton and buttonPressed functions.
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
#define BUTTON_MODE      0  // Move to the next mode
#define BUTTON_SEQUENCE  1  // Move to the next sequence of colours
#define BUTTON_EDIT      2  // Enter edit mode.
#define BUTTON_ADD       3  // Add a colour to the sequence
#define BUTTON_DELETE    4  // Delete a colour from a sequence


// **** BUTTON STATES ****

boolean button_states[ BUTTON_COUNT ];       // The state of the button (true => currently down)
boolean button_changes[ BUTTON_COUNT ];      // Used by buttonPressed to know when a button has just been pressed (as opposed to still DOWN from last time).
int button_previousReadings[ BUTTON_COUNT ]; // Used in conjuntion with button_debounceTimes to eliminate switch bounce.
long button_debounceTimes[ BUTTON_COUNT ];
long button_debounceMillis = 50;


// **** MODES ****
// There are "up" and "down" buttons to change modes, and each mode will display different types of patterns.

#define MODE_JUMP        0 // Cycle through a sequence of colours. Colours can be added by pressing the "Add" button.
#define MODE_FADE        1 // The same as MODE_JUMP, but fade between the colours (using a linear interpolation between pairs of colours).
#define MODE_FLASH       2 // Off inbetween each colour.
#define MODE_FADEOUT     3 // Fade to black inbetween each colour
#define MODE_DOUBLEFLASH 4 // Two unequal flashes (with black inbetween)
#define MODE_TWINKLE     5 // Rapid flashing on/off
#define MODE_ALTERNATE   6 // Rapid flashing between current colour and the previous one
#define MODE_EASECUBE    7 // Like fade, but change the shape of the "ease" curve, using subTick * subTick * subTick
#define MODE_WHITEOUT    8 // Like fladeout, but fade to white instead of black
                           // Add extra modes above, and change the next two lines
#define MODE_COUNT       9 // The number of modes

#define MODE_BROWSE     -1 // Not on of the normal modes. Enter browse mode using the edit button.
#define MODE_EDIT       -2 // When the edit button is pressed AGAIN, the current colour is editted (using the RGB pots).
#define MODE_ADD        -3
#define MODE_DELETE     -4 // When in MODE_BROWSE, the delete button will enter MODE_DELETE, where hitting delete again will delete the colour from the sequence.

int mode = MODE_JUMP;     // The current mode

// **** SEQUENCES ****

#define SEQUENCE_COUNT 10
#define MAX_SEQUENCE_LENGTH 20

int sequenceLengths[SEQUENCE_COUNT]; // The number of colours in each sequence
int sequenceValues[SEQUENCE_COUNT][MAX_SEQUENCE_LENGTH][3]; // The colours which make up all of the sequences

int sequence = 2; // The current sequence number

// Predefined sequences, which are populated during setup.
#define SEQUENCE_IMMEDIATE 0 
const byte SEQUENCE_WHITE[][3]   = { {255,255,255} };
const byte SEQUENCE_RGB[][3]     = { {255,0,0}, {0,255,0}, {0,0,255} };
const byte SEQUENCE_RGYBMCW[][3] = { {0,255,0}, {255,255,0}, {0,0,255}, {255,0,255}, {0,255,255}, {255,255,255}, {255,0,0} };
const byte SEQUENCE_HUES[][3]    = { {255,60,0}, {255,180,0}, {0,180,0}, {0,180,255}, {0,0,255}, {255,0,255}, {255,0,0} };
const byte SEQUENCE_INC_RGB[][3] = { {30,0,0}, {0,30,0}, {0,0,30}, {80,0,0}, {0,80,0}, {0,0,80}, {128,0,0}, {0,128,0}, {0,0,128}, {255,0,0}, {0,255,0}, {0,0,255} };
const byte SEQUENCE_RWB[][3]     = { {0,0,255}, {255,255,255}, {255,0,0}, {255,255,255} };
const byte SEQUENCE_OFF[][3]     = { {0,0,0} };

// **** TIMING ****
long lastTickMillis = 0;   // The time in millis of the start of the previous "tick"
int delayDuration = 900;   // The time between ticks (Will be adjusted using a potentiometer)

int tickNumber = 0;        // The current tick number, sequence and fade use this to choose which item in the sequence to display.
float subTick = 0.0;       // How far through the current tick are we (0..0.99999) 0 = tick just started, 0.99999 about to start the next tick.
int prevTickNumber = 0;    // The previous tickNumber, which will be tickNumber - 1, except when tickNumber has looped back to 0.

// **** EPROM LOCATIONS ****
#define EEPROM_PROG_VERSION 0
#define EEPROM_MAX_SEQ_LEN  1
#define EEPROM_SEQ_COUNT    2
#define EEPROM_SEQUENCE     3
#define EEPROM_MODE         4
#define EEPROM_DATA        10 // Start variable length data here.

// **** The channel numbers for each colour component ****
#define RED   0
#define GREEN 1
#define BLUE  2

// **** Use to turn debugging on/off
#define DEBUG

#ifdef DEBUG
  #define dprint(x)    Serial.print (x)
  #define dprintln(x)  Serial.println (x)
#else
  #define dprint(x)
  #define dprintln(x) 
#endif 


// ***** CODE ****

// the setup routine runs once when you press reset:
void setup()
{ 
  Serial.begin(9600);

  dprintln("setup");
  
  int i;
  
  pinMode( EDIT_LED_PIN, OUTPUT );
  
  pinMode(SPEED_PIN, INPUT);
  for( i = 0; i < 3; i ++ ) {
    pinMode(LED_PINS[i], OUTPUT);
    pinMode(RGB_IN_PINS[i], INPUT);
  }
  
  for ( i = 0; i < BUTTON_COUNT; i ++ ) {
    pinMode( BUTTON_PINS[i], INPUT );
    digitalWrite( BUTTON_PINS[i], HIGH); // turn on pullup resistors
    button_states[i] = false;
    button_changes[i] = false;
    button_previousReadings[i] = LOW;
    button_debounceTimes[i] = 0;
  }
  
  color( 255,0,0 );
  
  //dprintln( "setup" );
  //dprint( "Version : " ); dprint( PROGRAM_VERSION );
  //dprint( " " ); dprint( SEQUENCE_COUNT );
  //dprint( " " ); dprintln ( MAX_SEQUENCE_LENGTH );
  
  resetSequences();

  color( 255,255,0 );
  
  if ( digitalRead( BUTTON_PINS[ BUTTON_DELETE ] ) == LOW ) {
    color( 0,0,255 );
    dprintln( "Forced Reset" );
    save();
  
  } else {
    color( 0,255,255 );
    //load();
    color( 255,0,255 );
  }
 
  setupMode();
    
  color( 0,255,0 );
  dprintln( "end setup" );
} 

void resetSequences()
{
  setSequence( 0, 1, SEQUENCE_WHITE ); // This is SEQUENCE_IMMEDIATE, which doesn't really have a sequence.
  setSequence( 1, 1, SEQUENCE_WHITE );
  setSequence( 2, 3, SEQUENCE_RGB );
  setSequence( 3, 7, SEQUENCE_RGYBMCW );
  setSequence( 4, 7, SEQUENCE_HUES );
  setSequence( 5, 12, SEQUENCE_INC_RGB );
  setSequence( 6, 4, SEQUENCE_RWB );
  setSequence( 7, 1, SEQUENCE_OFF );
  int i;
  for ( i = 7; i < SEQUENCE_COUNT; i ++ ) {
    setSequence( i, 1, SEQUENCE_WHITE );
  }
  
}

void setSequence( int s, byte n, const byte values[][3] )
{
  int i, j;

  sequenceLengths[s] = n;
  for ( i = 0; i < n; i ++ ) {
    for ( j = 0; j < 3; j ++ ) {
      sequenceValues[s][i][j] = values[i][j];
    }
  }
}


// the loop routine runs over and over again forever:
void loop()
{
  
  events();

}


void setupMode()
{
  dprint( "Mode : " ); dprintln( mode );

  prevTickNumber = 0;
  tickNumber = 0;
  lastTickMillis = millis();
    
  dprint( "Sequence " );
  dprint( sequence );
  dprint( " : " );
  int i, j;
  for ( i = 0; i < sequenceLengths[sequence]; i ++ ) {
    for ( j = 0; j < 3; j ++ ) {
      dprint( sequenceValues[sequence][i][j] ); dprint( " " );
    }
    dprint(" - ");
  }
  dprintln("");
    
  // Save to EEPROM, so that when turned off & on, it returns to the same pattern.
  pokeByte( EEPROM_SEQUENCE, sequence );
  pokeByte( EEPROM_MODE, mode );
      
}


void events()
{
  long now = millis();
  long ellapsed = now - lastTickMillis;
  subTick = ((float) ellapsed) / delayDuration;
  if (subTick > 0.9999) {
    subTick = 0.9999;
  }
 
  readButtons();

  long speedReading = analogRead( SPEED_PIN );
  delayDuration = (speedReading + 100) * speedReading / 50 ;

  // The zeroth sequence is special... It always displays what is on the RGB dials (potentiometers).
  if ( sequence == SEQUENCE_IMMEDIATE ) {
    setColourFromDials();
  }

  if ( ellapsed > delayDuration ) {
    lastTickMillis = millis();
    prevTickNumber = tickNumber;

    // Don't increment th tick while in edit modes.
    if ( mode >= 0 ) {
      tickNumber = tickNumber + 1;
        
      if ( tickNumber >= sequenceLengths[sequence] ) {
        tickNumber = 0;
      }
    }
  }
    
  if ( mode >= 0 ) {
    
    if ( buttonPressed( BUTTON_MODE ) ) {
      mode = mode + 1;
      if ( mode >= MODE_COUNT ) {
        mode = 0;
      }
      setupMode();
    }
    
    if ( buttonPressed( BUTTON_SEQUENCE ) ) {
      sequence = sequence + 1;
      if ( sequence >= SEQUENCE_COUNT ) {
        sequence = 0;
      }
      setupMode();
    }
    
    if ( buttonPressed( BUTTON_EDIT ) ) {
      if (sequence != SEQUENCE_IMMEDIATE) {
        beginBrowse();
      }
    }
  }
  
  if ( mode == MODE_JUMP ) {
    jump_events();
  } else if ( mode == MODE_FADE ) {
    fade_events();
  } else if ( mode == MODE_FLASH ) {
    flash_events();
  } else if ( mode == MODE_FADEOUT ) {
    fadeout_events();
  } else if ( mode == MODE_DOUBLEFLASH ) {
    doubleflash_events();
  } else if ( mode == MODE_TWINKLE ) {
    twinkle_events();
  } else if ( mode == MODE_ALTERNATE ) {
    alternate_events();
  } else if ( mode == MODE_EASECUBE ) {
    easecube_events();
  } else if ( mode == MODE_WHITEOUT ) {
    whiteout_events();
    
  } else if ( mode == MODE_BROWSE ) {
    browse_events();
  } else if ( mode == MODE_EDIT ) {
    edit_events();
  } else if ( mode == MODE_ADD ) {
    add_events();
  } else if ( mode == MODE_DELETE ) {
    delete_events();
  }

}



void setColourFromDials()
{
  int i;
  
  dprint( "Using dials " );
  for ( i = 0; i < 3; i ++ ) {
    dprint( getChannel( i ) ); dprint( " " );
    setChannel(i, getChannel( i ) );
  }
  dprintln();
}

int getChannel( int c )
{
  return analogRead(RGB_IN_PINS[c]) / 4;
}

// Adapted from the "Debounce" script
// Read the state of all buttons, handling debounce. The debounced results are in the button_state array.
void readButtons()
{
  int i;
  long now = millis();
  
  for ( i = 0; i < BUTTON_COUNT; i ++ ) {
    
    boolean reading = (digitalRead( BUTTON_PINS[ i ]) == LOW);
    // check to see if you just pressed the button 
    // (i.e. the input went from LOW to HIGH),  and you've waited 
    // long enough since the last press to ignore any noise:  
  
    // If the switch changed, due to noise or pressing:
    if (reading != button_previousReadings[i]) {
      // reset the debouncing timer
      button_debounceTimes[i] = now;
    } 
    
    if ((now - button_debounceTimes[i]) > button_debounceMillis) {
      // whatever the reading is at, it's been there for longer
      // than the debounce delay, so take it as the actual current state:
      // dprint( "Button not a bounce " ); dprint( i ); dprint( " = " ); dprintln( reading ); dprint( " was " ); dprintln( button_states[i] );

      if ( reading != button_states[i] ) {
        button_changes[i] = true;
        dprint( "Changed: button[" ); dprint( i ); dprint( "] = " ); dprintln( reading );
      }
      button_states[i] = reading;
    }
    
    // Remember the current reading for the next time this function is called.
    button_previousReadings[i] = reading;

  }
}

// Check if the button is DOWN, and has been UP since we last called this function.
boolean buttonPressed( int button )
{
  if (button_states[button]) {
    if (button_changes[button]) {
      button_changes[button] = false;
      return true;
    }
  }
  return false;
}

void color( int r, int g, int b )
{
  setChannel( RED, r );
  setChannel( GREEN, g );
  setChannel( BLUE, b );
}

void setChannel( int channel, int value )
{
  if ( value < 0 ) {
    value = 0;
  }
  if ( value > 255 ) {
    value = 255;
  }
  analogWrite( LED_PINS[channel], 255 - value );
}


void jump_events()
{
  int i;
    
  //dprint("RGB: ");
  for ( i = 0; i < 3; i ++ ) {
    int val = sequenceValues[sequence][tickNumber][i];
    setChannel( i, val );
    //dprint( sequenceValues[sequence][tickNumber][i] ); dprint( " " );
  }
  //dprintln("");
}


void fade_events()
{
  /*
  dprint( "Fade events. Prev #" );  
  dprint( prevTickNumber );
  dprint( "Tick #" );  
  dprint( tickNumber );
  dprintln();
  */
  int i;
  
  //dprint( "Fading " );
  for ( i = 0; i < 3; i ++ ) {
    int prev = sequenceValues[sequence][prevTickNumber][i];
    int next = sequenceValues[sequence][tickNumber][i];
    
    int value = (1.0-subTick) * prev + subTick * next;
    //dprint( value );
    //dprint( " " );
    setChannel( i, value);
  }
  //dprintln()
}

void flash_events()
{
  int i;
  
  if ( subTick > 0.5 ) {
    color( 0,0,0 );
  } else {
    for ( i = 0; i < 3; i ++ ) {
      int val = sequenceValues[sequence][tickNumber][i];
      
      setChannel( i, val);
    }
  }
}

void fadeout_events()
{
  int i;
  
  if ( subTick < 0.2 ) {
    
    for ( i = 0; i < 3; i ++ ) {
      int val = sequenceValues[sequence][tickNumber][i];
      
      int value = subTick * 5 * val;
      setChannel( i, value);
    }
 
  } else if ( subTick > 0.8 ) {
    
    for ( i = 0; i < 3; i ++ ) {
      int val = sequenceValues[sequence][tickNumber][i];
      
      int value = (1 - subTick) * 5 * val;
      setChannel( i, value);
    }
  }
  
}


void whiteout_events()
{
  int i;
  
  if ( subTick < 0.2 ) {
    
    for ( i = 0; i < 3; i ++ ) {
      int val = sequenceValues[sequence][tickNumber][i];
      float sub = subTick * 5;
      int value = (1.0-sub) * 255 + sub * val;
      setChannel( i, value);
    }
 
  } else if ( subTick > 0.8 ) {
  
    for ( i = 0; i < 3; i ++ ) {
      int val = sequenceValues[sequence][tickNumber][i];
      float sub = (1 - subTick) * 5;

      int value = (1.0-sub) * 255 + sub * val;
      setChannel( i, value);
    }
  
  }
  
}


void doubleflash_events()
{
  int i;
  
  if ( (subTick > 0.1) && (subTick < 0.2) ) {
    color( 0,0,0 );
  } else if ( (subTick > 0.4) && (subTick < 0.5) ) {
    color( 0,0,0 );
  } else {
    for ( i = 0; i < 3; i ++ ) {
      int val = sequenceValues[sequence][tickNumber][i];
      
      setChannel( i, val);
    }
  }  
}


void twinkle_events()
{
  int i;

  int part = (subTick * 100);

  int sub = part % 10;  
  if ( sub < 3 ) {
    color( 0,0,0 );
  } else {
    for ( i = 0; i < 3; i ++ ) {
      int val = sequenceValues[sequence][tickNumber][i];
      
      setChannel( i, val);
    }
  }  
}


void alternate_events()
{
  int i;

  int part = (subTick * 91);

  int sub = part % 9;  
  if ( sub < 3 ) {
    for ( i = 0; i < 3; i ++ ) {
      int val = sequenceValues[sequence][prevTickNumber][i];
      
      setChannel( i, val);
    }
  } else {
    for ( i = 0; i < 3; i ++ ) {
      int val = sequenceValues[sequence][tickNumber][i];
      
      setChannel( i, val);
    }
  }  
}

void easecube_events()
{
  subTick = subTick * subTick * subTick;
  fade_events();
}


int previousMode = 0;

void beginBrowse()
{
  dprintln( "Begin Browse mode" );
  previousMode = mode;
  mode = MODE_BROWSE;
  setupMode();
  
}

void endBrowse()
{
  digitalWrite( EDIT_LED_PIN, LOW );

  mode = previousMode;
  setupMode();
}


void checkForExit()
{
  if ( buttonPressed( BUTTON_MODE ) ) {
    endBrowse();
  }
}

void browse_events()
{
  dprintln( "Browse events" );
  
  digitalWrite( EDIT_LED_PIN, HIGH );
  
  int i;

  // SEQUENCE button moves to the next colour in the sequence.
  if ( buttonPressed( BUTTON_SEQUENCE ) ) {
    tickNumber = tickNumber + 1;
    if ( tickNumber >= sequenceLengths[sequence] ) {
      tickNumber = 0;
    }
  }
  
  if ( buttonPressed( BUTTON_EDIT ) ) {
    mode = MODE_EDIT;
  }

  if ( buttonPressed( BUTTON_DELETE ) ) {
    mode = MODE_DELETE;
  }
  
  if ( buttonPressed( BUTTON_ADD ) ) {
    mode = MODE_ADD;
  }
  
  checkForExit();
  
  for ( i = 0; i < 3; i ++ ) {
    int val = sequenceValues[sequence][tickNumber][i];
      
    setChannel( i, val);
  }  
}


int rangedPeekByte( int address, int minVal, int maxVal, int defaultVal )
{
  
  int value = EEPROM.read(address);
  if ( (value < minVal) || (value > maxVal) ) {
    return defaultVal;
  }
  return value;
}

int peekByte( int address )
{
  return EEPROM.read(address);
}

void pokeByte( int address, int value )
{
  EEPROM.write(address, value);
}

int eepromAddress = 0;
void seek( int address )
{
  eepromAddress = address;
}

int readByte()
{
  return peekByte( eepromAddress ++ );
}
void writeByte( int value )
{
  pokeByte( eepromAddress ++, value );
}


// Load sequence data from EEPROM
void load()
{
  int s, n, i;
  
  dprintln( "Load" );
 
  int progVersion = peekByte( EEPROM_PROG_VERSION );
  int seqCount = peekByte( EEPROM_SEQ_COUNT );
  int maxSeqLen = peekByte( EEPROM_MAX_SEQ_LEN );
    
  if ( (progVersion != PROGRAM_VERSION) || (maxSeqLen != MAX_SEQUENCE_LENGTH) || (seqCount != SEQUENCE_COUNT) ) {
    
    resetSequences();
    save();
    
  } else {
  
    // Return to the same mode and sequence from when last used.
    sequence = rangedPeekByte( EEPROM_SEQUENCE, 0, SEQUENCE_COUNT - 1, 0 );
    mode = rangedPeekByte( EEPROM_MODE, 0, MODE_COUNT - 1, 0 );
  
    seek( EEPROM_DATA );
    for ( s = 0; s < SEQUENCE_COUNT; s ++ ) {
      int len = readByte();
      if ( (len < 1) || (len > MAX_SEQUENCE_LENGTH) ) {
        dprintln( "Failed to read EEPROM data" );
      }
      sequenceLengths[s] = len;
      
      for ( n = 0; n < sequenceLengths[s]; n ++ ) {
        for ( i = 0; i < 3; i ++ ) {
          sequenceValues[s][n][i] = readByte();
        }
      }
    }
  }
  
}

void save()
{
  int s, n, i;
  
  dprintln( "Save" );
  
  pokeByte( EEPROM_PROG_VERSION, PROGRAM_VERSION );
  pokeByte( EEPROM_MAX_SEQ_LEN, MAX_SEQUENCE_LENGTH );
  pokeByte( EEPROM_SEQ_COUNT, SEQUENCE_COUNT );
  
  seek( EEPROM_DATA );
  for ( s = 0; s < SEQUENCE_COUNT; s ++ ) {
    writeByte( sequenceLengths[s] );
    
    for ( n = 0; n < sequenceLengths[s]; n ++ ) {
      for ( i = 0; i < 3; i ++ ) {
        writeByte( sequenceValues[s][n][i] );
      }
    }
  }

}


void edit_events()
{
  dprintln( "Edit events" );

  digitalWrite( EDIT_LED_PIN, ((int) (subTick  * 3)) % 2  ); // Single flash
  
  setColourFromDials();
  
  int i;
  if ( buttonPressed( BUTTON_EDIT ) ) {
    dprintln( "Saving colour" );
    for ( i = 0; i < 3; i ++ ) {
      sequenceValues[sequence][tickNumber][i] = getChannel( i );
    }
    save();
    mode = MODE_BROWSE;
  }

  checkForExit();
}

void add_events()
{
  int n, i;

  dprintln( "Add" );

  digitalWrite( EDIT_LED_PIN, ((int) (subTick  * 5)) % 2  ); // Double flash

  // Can't exceed the max number of colours in a sequence.
  if ( sequenceLengths[sequence] >= MAX_SEQUENCE_LENGTH ) {
    mode = MODE_BROWSE;
    return;
  }

  if ( buttonPressed( BUTTON_ADD ) ) {
    
    // Move the colour up one. (start for the END and work backwards)
    for (n = sequenceLengths[sequence] - 1; n >= tickNumber; n -- ) {
      for (i = 0; i < 3; i ++ ) {
        sequenceValues[sequence][n+1][i] = sequenceValues[sequence][n][i];
      }
    }
    sequenceLengths[sequence] ++;
    
    for ( i = 0; i < 3; i ++ ) {
      sequenceValues[sequence][tickNumber][i] = getChannel( i );
    }
    save();   
    mode = MODE_BROWSE;
  }

  setColourFromDials();

  checkForExit();
}

void delete_events()
{
  int n, i;

  dprintln( "Delete" );
  
  digitalWrite( EDIT_LED_PIN, ((int) (subTick  * 7)) % 2  ); // Triple flash

  // Can't delete the only colour in a sequence!
  if ( sequenceLengths[sequence] < 2 ) {
    mode = MODE_BROWSE;
    return;
  }

  if ( subTick < 0.1 ) {
    color( 0,0,0 );
  } else if ( subTick < 0.2 ) {
    color( 255,0,0 );
  } else if ( subTick < 0.3 ) {
    color( 0,0,0 );
  } else {
    for ( i = 0; i < 3; i ++ ) {
      int val = sequenceValues[sequence][tickNumber][i];
      
      setChannel( i, val);
    }
  }
  
  if ( buttonPressed( BUTTON_DELETE ) ) {
    // Move all the later colours down by one
    for ( n = tickNumber + 1; n < sequenceLengths[sequence]; n ++ ) {
      for ( i = 0; i < 3; i ++ ) {
        sequenceValues[sequence][n-1][i] = sequenceValues[sequence][n][i];
      }
    }
    sequenceLengths[sequence] --;
    tickNumber = tickNumber -1;
    if (tickNumber < 0) {
      tickNumber = sequenceLengths[sequence] -1;
    }
    save();
    mode = MODE_BROWSE;
  }
  
  checkForExit();
  
}


