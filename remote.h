#ifndef HEADER_REMOTE
#define HEADER_REMOTE

#include <Arduino.h>
#include "NTC_IRremote.h"

class Remote
{
  public :
    IRrecv receiver;  

  public :
    Remote( int pin );
    
    void setup();
    void checkButtons();
    
  private :
    void action( long button );
    void showColor( byte red, byte green, byte blue );
    void showColor( long color );
};

#endif
