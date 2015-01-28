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
    void loop();
    long getButton();
};


#define REMOTE_BRIGHTNESS_UP   0xFF3AC5
#define REMOTE_BRIGHTNESS_DOWN 0xFFBA45
#define REMOTE_PAUSE           0xFF827D
#define REMOTE_POWER           0xFF02FD

#define REMOTE_RED             0xFF1AE5
#define REMOTE_GREEN           0xFF9A65
#define REMOTE_BLUE            0xFFA25D
#define REMOTE_WHITE           0xFF22DD

#define REMOTE_ORANGE          0xFF2AD5
#define REMOTE_LGREEN          0xFFAA55
#define REMOTE_LBLUE           0xFF926D
#define REMOTE_PINK            0xFF12ED

#define REMOTE_ORANGE2         0xFF0AF5
#define REMOTE_CYAN            0xFF8A75
#define REMOTE_PURPLE          0xFFB24D
#define REMOTE_PINK2           0xFF32CD

#define REMOTE_ORANGE3         0xFF38C7
#define REMOTE_TEAL            0xFFB847
#define REMOTE_AUBERGINE       0xFF7887
#define REMOTE_PALE_BLUE       0xFFF807

#define REMOTE_YELLOW          0xFF18E7
#define REMOTE_TEAL2           0xFF9867
#define REMOTE_MAUVE           0xFF58A7
#define REMOTE_PALE_BLUE2      0xFFD827

#define REMOTE_RED_UP          0xFF28D7
#define REMOTE_GREEN_UP        0xFFA857
#define REMOTE_BLUE_UP         0xFF6897
#define REMOTE_QUICK           0xFFE817

#define REMOTE_RED_DOWN        0xFF08F7
#define REMOTE_GREEN_DOWN      0xFF8877
#define REMOTE_BLUE_DOWN       0xFF48B7
#define REMOTE_SLOW            0xFFC837

#define REMOTE_DIY1            0xFF30CF
#define REMOTE_DIY2            0xFFB04F
#define REMOTE_DIY3            0xFF708F
#define REMOTE_AUTO            0xFFF00F

#define REMOTE_DIY4            0xFF10EF
#define REMOTE_DIY5            0xFF906F
#define REMOTE_DIY6            0xFF50AF
#define REMOTE_FLASH           0xFFD02F

#define REMOTE_JUMP3           0xFF20DF
#define REMOTE_JUMP7           0xFFA05F
#define REMOTE_FADE3           0xFF609F
#define REMOTE_FADE7           0xFFE01F

#endif

