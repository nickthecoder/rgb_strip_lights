#include <Arduino.h>
#include "remote.h"
#include "controller.h"

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

#define REMOTE_DIY1            OxFF30CF
#define REMOTE_DIY2            0xFFB04F
#define REMOTE_DIY3            0xFF708F
#define REMOTE_AUTO            0xFFF00F

#define REMOTE_DIY4            0xFF10EF
#define REMOTE_DIY5            0xFF906F
#define REMOTE_DIY6            0xFF50AF
#define REMOTE_FLASH           0xFFD02F

#define REMOTE_JUMP3           0xFF20DF
#define REMOTE_JUMP7           0xFFA05F
#define REMOTE_FADE3           OxFF609F
#define REMOTE_FADE7           0xFFE01F


Remote::Remote( int pin )
  : receiver(IRrecv(pin))
{
}

void Remote::setup()
{
  dprintln( "IR Setup" );
  receiver.enableIRIn(); // Start the receiver
}

decode_results results;

void Remote::checkButtons()
{
  boolean result = receiver.decode(&results);
  if (result) {
    // dhexvalue( "Remote : ", results.value);
    
    action( results.value );
    
    receiver.resume(); // Receive the next value
  }
  
}

void Remote::action( long button )
{
  dhexvalue( "Action buttons", button );

  if ( button == REMOTE_POWER) {
    showColor( 0x050000 );

  } else if ( button == REMOTE_PAUSE) {
      if ( controller.pMode == &stayMode ) {
          controller.setMode( controller.modeIndex );
      } else {
          controller.setMode( &stayMode );
      }

  } else if ( button == REMOTE_RED ) {
    showColor( 0xff0000 );
  } else if ( button == REMOTE_GREEN ) {
    showColor( 0x00ff00 );
  } else if ( button == REMOTE_BLUE ) {
    showColor( 0x0000ff );
  } else if ( button == REMOTE_WHITE ) {
    showColor( 0xffffff );
  } else if ( button == REMOTE_ORANGE ) {
    showColor( 0xff6600 );
  } else if ( button == REMOTE_LGREEN ) {
    showColor( 0xb4ff00 );
  } else if ( button == REMOTE_LBLUE ) {
    showColor( 0x0093ff );
  } else if ( button == REMOTE_PINK ) {
    showColor( 0xf9a860 );
  } else if ( button == REMOTE_ORANGE2 ) {
    showColor( 0xffd800 );
  } else if ( button == REMOTE_CYAN ) {
    showColor( 0x00ffff );
  } else if ( button == REMOTE_PURPLE ) {
    showColor( 0x6700ff );
  } else if ( button == REMOTE_PINK2 ) {
    showColor( 0xf9a8b0 );
  } else if ( button == REMOTE_ORANGE3 ) {
    showColor( 0xffd24a );
  } else if ( button == REMOTE_TEAL ) {
    showColor( 0x4393ba );
  } else if ( button == REMOTE_AUBERGINE ) {
    showColor( 0xa923c5 );
  } else if ( button == REMOTE_PALE_BLUE ) {
    showColor( 0xa1c6ff );
  } else if ( button == REMOTE_YELLOW ) {
    showColor( 0xffff00 );
  } else if ( button == REMOTE_TEAL2 ) {
    showColor( 0x3b5e72 );
  } else if ( button == REMOTE_MAUVE ) {
    showColor( 0xf310b4 );
  } else if ( button == REMOTE_PALE_BLUE2 ) {
    showColor( 0xa1c6ff );
  }
  
}

void Remote::showColor( byte red, byte green, byte blue )
{
    controller.color( red, green, blue );
    controller.setMode( &stayMode );
}

void Remote::showColor( long color )
{
    byte red = color >> 16;
    byte green = (color >> 8) & 0xFF;
    byte blue = color & 0xFF;
    
    Serial.print( "Color " ); Serial.print( red ); Serial.print( " " ); Serial.print( green ); Serial.print( " " ); Serial.println( blue );
    controller.color( red, green, blue );
    controller.setMode( &stayMode );
}

