#include <Arduino.h>

#include "debug.h"
#include "standardModes.h"
#include "controller.h"


#define EASE(s) controller.getEase()->ease(s)

void color(  byte* rgb )
{
    //dprintln( "Color " );
    controller.color( rgb );
}

void merge( byte* rgb1, byte* rgb2, float ratio )
{
    //dprint( "Merge " ); dprint( ratio ); dprint( " : " );
    for ( int c = 0; c < 3; c ++ ) {
        byte value = ratio * rgb1[c] + (1-ratio) * rgb2[c];
        //dprint( value ); dprint( " " );
        controller.channel( c, value );
    }
    //dprintln( "" );
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
    dvalue( "Part", part);
  
    if ( (part == 0) || (part == 2) ) {
        merge( getColor(), BLACK, subSub );
    } else if ( (part == 1) || (part == 3) ) {
        merge( BLACK, getColor(), subSub );
    } else {
        color( BLACK );
    }
}
/*
Like Fadeeout, but uses the value on the dials
*/
void Dialout::display( float subTicks )
{    
    if ( subTicks < 0.5 ) {
        merge( getColor(), controller.getDialColor(), EASE(subTicks * 2) );
    } else {
        merge( getColor(), controller.getDialColor(), EASE( (1 - subTicks) * 2 ) );
    }
}


Multiple::Multiple( SequenceMode* pRepeatedMode, int times ) : SequenceMode()
{
    this->pRepeatedMode = pRepeatedMode;
    this->times = times;
}

void Multiple::display( float subTicks )
{
    subTicks = subTicks * times;
    int i = (int) subTicks;
    subTicks = subTicks - i;

    pRepeatedMode->display( subTicks );   
}

void Multiple::nextTick()
{
    SequenceMode::nextTick();
    pRepeatedMode->nextTick();
}

FixedPeriod::FixedPeriod( long period )
{
    this->period = period;
}

void FixedPeriod::begin()
{
    SequenceMode::begin();
    lastTime = millis();
}

void FixedPeriod::display( float subTicks )
{
    long now = millis();
    if (now > lastTime + period) {
        lastTime = now;
    }
    
    float subPeriod = ((float) (now - lastTime)) / period;
    displaySubPeriod( controller.getEase()->ease( subPeriod ) );
}

Twinkle::Twinkle( long period ) : FixedPeriod( period )
{
}

void Twinkle::displaySubPeriod( float subPeriod )
{
    if ( subPeriod < 0.5 ) {
        merge( BLACK, getColor(), EASE( subPeriod * 2 ) );
    } else {
        merge( getColor(), BLACK, EASE( (subPeriod - 0.5) * 2) );
    }
}


Alternate::Alternate( long period ) : FixedPeriod( period )
{
}

void Alternate::displaySubPeriod( float subPeriod )
{
    if (subPeriod < 0.5) {
        merge( getPreviousColor(), getColor(), EASE( subPeriod * 2 ) );
    } else {
        merge( getColor(), getPreviousColor(), EASE( (subPeriod - 0.5) * 2) );
    }
}

