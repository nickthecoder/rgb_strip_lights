#include <Arduino.h>

#include "debug.h"
#include "standardModes.h"
#include "controller.h"


#define EASE(s) controller.getEase()->ease(s)

void color(  byte* rgb )
{
    controller.color( rgb );
}

void merge( byte* rgb1, byte* rgb2, float ratio )
{
    for ( int c = 0; c < 3; c ++ ) {
        byte value = ratio * rgb1[c] + (1-ratio) * rgb2[c];
        controller.channel( c, value );
    }
}  



/*
Fade from one colour to the next
*/
void Fade::display( float subTicks )
{
    merge( getColor(), getPreviousColor(), EASE(subTicks) );
}

/*
Fade to black between each colour
*/
void Fadeout::display( float subTicks )
{
    if ( subTicks < 0.5 ) {
        merge( getColor(), BLACK, EASE(subTicks * 2) );
    } else {
        merge( getColor(), BLACK, EASE( (1 - subTicks) * 2 ) );
    }
}

/*
Like Fadeout, but fade to white between each colour
*/
void Whiteout::display( float subTicks )
{    
    float front = 0.25;
    float back = 1 - front;
    float scale = 1 / front;
        
    if ( subTicks < front ) {
        merge( getColor(), WHITE, EASE( subTicks * scale ) );
    } else if ( subTicks < back ) {
        color( getColor() );
    } else {
        merge( WHITE, getColor(), EASE( (subTicks - back) * scale ) );
    }
}

/*
Gives 2 short beats followed by a longer delay
*/
void HeartBeat::display( float subTicks )
{
    int length = 5; // High value gives more OFF time.
    
    int part = subTicks * length;
    float subSub = subTicks * length - part;
  
    if ( (part == 0) || (part == 2) ) {
        merge( getColor(), BLACK, subSub );
    } else if ( (part == 1) || (part == 3) ) {
        merge( BLACK, getColor(), subSub );
    } else {
        color( BLACK );
    }
}

void Periodic::begin()
{
    SequenceMode::begin();
    lastTime = millis();
}

void Periodic::display( float subTicks )
{
    long now = millis();
    float period = getPeriod();
    if (now > lastTime + period) {
        lastTime = now;
    }
    
    float subPeriod = ((float) (now - lastTime)) / period;
    displaySubPeriod( subPeriod );
}

int Periodic::getPeriod()
{
    long potValue = (1024 - analogRead( TWINKLE_PIN ) );
    long value = 60 + potValue * 4;
    return value;
}

void Periodic::drawScreen()
{
    EasingSequenceMode::drawScreen();

    float duration = getPeriod();
    float x = 128 - ( duration - 60 ) / 32;
    if ( x < 1 ) {
      x = 1;
    }
    if ( x > 127 ) {
      x = 127;
    }
    u8g.drawBox( 0, 60, (int) x, 3 );
}


void Twinkle::displaySubPeriod( float subPeriod )
{
    if ( subPeriod < 0.5 ) {
        merge( BLACK, getColor(), EASE( subPeriod * 2.0 ) );
    } else {
        merge( BLACK, getColor(), EASE( (1-subPeriod) * 2.0) );
    }
}


void Alternate::displaySubPeriod( float subPeriod )
{
    if (subPeriod < 0.5) {
        merge( getPreviousColor(), getColor(), EASE( subPeriod * 2 ) );
    } else {
        merge( getPreviousColor(), getColor(), EASE( (1 - subPeriod) * 2) );
    }
}

byte* getLastColor()
{
    return controller.sequence.colors[ controller.sequence.length - 1 ];
}

void AlternateLast::displaySubPeriod( float subPeriod )
{
    if (subPeriod < 0.5) {
        merge( getLastColor(), getColor(), EASE( subPeriod * 2 ) );
    } else {

        merge( getLastColor(), getColor(), EASE( (1 - subPeriod) * 2) );
    }
}

void AlternateLast::nextTick()
{
    Periodic::nextTick();
    if (colorIndex >= controller.sequence.length - 1) {
        colorIndex = 0;
    }
}

void StaticMode::display( float subPeriod )
{
    color( controller.getDialColor() );
}


StayMode stayMode = StayMode();
PauseMode pauseMode = PauseMode();


