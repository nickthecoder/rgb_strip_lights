#include "debug.h"
#include "controller.h"

#define DEBOUNCE_MILLIS 50

int Button::tonePin = 5;
int Button::toneDuration = 100;
int Button::toneFrequency = 440;

Button::Button( int pin )
{
    this->pin = pin;

    down = false;
    changed = false;
    previousReading = false;
    stableTime = millis();
    
    pinMode( pin, INPUT );
    digitalWrite( pin, HIGH ); // turn on pullup resistors
}
    
boolean Button::raw()
{
    return (digitalRead( pin ) == LOW);
}

boolean Button::isDown()
{
    boolean reading = (digitalRead( pin ) == LOW);

    long now = millis();
    
    // If the switch changed, due to noise or pressing:
    if (reading != previousReading) {
        // reset the debouncing timer
        stableTime = now;
    }
    
    if (now > stableTime + DEBOUNCE_MILLIS) {
        // whatever the reading is at, it's been there for longer
        // than the debounce delay, so take it as the actual current state:
        if ( down != reading ) {
            down = reading;
            changed = true;
            dprint( "Changed: " ); dprint( pin ); dprint( " : " ); dprintln( reading );
        }
    }

     
    // Remember the current reading for the next time this function is called.
    previousReading = reading;
    
    return down;
}

boolean Button::keyPressed()
{
    boolean wasDown = isDown();
    if (changed) {
        changed = false;

        return ! wasDown;
    }
  
    return false;
}


RemoteInput::RemoteInput( long button )
{
    this->button = button;
}

boolean RemoteInput::keyPressed()
{
    return controller.remote.getButton() == button;
}

DualInput::DualInput( Input* pA, Input* pB )
{
    this->pA = pA;
    this->pB = pB;
}

boolean DualInput::keyPressed()
{
    return pA->keyPressed() || pB->keyPressed();
}

