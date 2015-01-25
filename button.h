#include <Arduino.h>

#ifndef HEADER_BUTTON
#define HEADER_BUTTON
   
class Button {
  
  public :
  
    Button( int pin );

    boolean raw();           // Return the state of the key without debouncing the result.
    boolean isDown();        // return true iff the button is current down
    boolean keyPressed();    // return true iff the button has just completed an "up" i.e. is now up, but was down.
   
  private :
    int pin;                 // The pin number used for the digitalRead
    boolean down;            // The state of the button. Is it down?
    boolean changed;         // Has the button state changed since it was last read?
    
    // Usd to de-bounce the button
    boolean previousReading; // The previous raw reading from digitalRead
    long stableTime;      // The time in millis when the button was last stable i.e. not fluctuating caused by button "bounce"
};



#endif

