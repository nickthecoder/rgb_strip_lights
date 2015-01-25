#include <Arduino.h>

#include "debug.h"
#include "controller.h"

Controller controller = Controller();

int rgbPins[] = { RED_OUT_PIN, GREEN_OUT_PIN, BLUE_OUT_PIN };
int rgbInPins[] = { RED_IN_PIN, GREEN_IN_PIN, BLUE_IN_PIN };

Fade fade = Fade();
Fadeout fadeout = Fadeout();
HeartBeat heartBeat = HeartBeat();
Whiteout whiteout = Whiteout();
Multiple doubleFadeout = Multiple( &fadeout, 2 );
Multiple quadFadeout = Multiple( &fadeout, 4 );
Multiple doubleWhiteout = Multiple( &whiteout, 2 );
Multiple quadWhiteout = Multiple( &whiteout, 4 );
Twinkle twinkle = Twinkle( 70 );
Twinkle slowTwinkle = Twinkle( 200 );
Alternate alternate = Alternate( 100 );
Alternate slowAlternate = Alternate( 300 );
Dialout dialout = Dialout();

#define MODE_COUNT 13
Mode* pModes[MODE_COUNT] = { &fade, &fadeout, &heartBeat, &whiteout, &doubleFadeout, &quadFadeout, &doubleWhiteout, &quadWhiteout, &twinkle, &slowTwinkle, &alternate, &slowAlternate, &dialout };
int getModeCount() {
    return MODE_COUNT;
}

#define EASE_COUNT 8
Ease* pEases[EASE_COUNT] = {  &jump, &linear, &easeInQuad, &easeOutQuad, &easeInCubic, &easeOutCubic, &easeInQuart,  &easeOutQuart };
int getEaseCount() {
    return EASE_COUNT;
}

Sequence sequence = Sequence();


Controller::Controller()
    : modeButton( Button( MODE_PIN ) )
    , sequenceButton( Button( SEQUENCE_PIN ) )
    , easeButton( Button( EASE_PIN ) )

    , editButton( Button( EDIT_PIN ) )
    , addButton( Button( ADD_PIN ) )
    , deleteButton( Button( DELETE_PIN ) )
{

    modeIndex = 0;
    easeIndex = 0; //EASE_COUNT - 1;
    sequenceIndex = 0;

    sequence.a( 255,255,255 );
}

void Controller::setup()
{
    pinMode( SPEED_PIN, INPUT );
    
    pinMode( RED_OUT_PIN, OUTPUT );
    pinMode( GREEN_OUT_PIN, OUTPUT );
    pinMode( BLUE_OUT_PIN, OUTPUT );
    
    color( 5, 1, 0 ); // Amber... On your marks...

    //#ifdef DEBUG
    Serial.begin( 9600 );
    //#endif

    dprintln( "Setup" );

    setMode( 0 );

    dvalue( "Reset ? ", deleteButton.raw() );
    if ( deleteButton.raw() ) {
        dprintln( "User Reset" );
        resetData();
    } else {

        if ( ! load() ) {
            dprintln( "Forced Reset" );
            resetData();
        }
    }

    color( 0, 5, 0 ); // Green... Go.

}
    
void Controller::resetData()
{
    save();
    reset();

    sequence.clear().a( 255, 0, 0 ).a( 0, 255, 0 ).a( 0, 0, 255 );
    saveSequence( 0, &sequence );

    sequence.clear().a( 255,0,0 ).a( 255,60,0 ) .a( 255,180,0 ).a( 0,180,0 ).a( 0,180,255 ).a( 0,0,255 ).a( 255,0,255 );
    saveSequence( 1, &sequence );

    sequence.clear().a( 0,255,0 ) .a( 255,255,0 ).a( 0,0,255 ).a( 255,0,255 ).a ( 0,255,255 ).a( 255,255,255 ).a( 255,0,0 );
    saveSequence( 2, &sequence );

    sequence.clear().a(0,0,255).a(255,255,255).a(255,0,0).a(255,255,255);
    saveSequence( 3, &sequence );
    
    sequence.clear().a(60,60,255).a(255,255,255);
    saveSequence( 4, &sequence );

    sequence.clear();
    for ( int i = 10; i <= 255; i += 10 ) {
        sequence.a( i,0,0 );
    }
    for ( int i = 255; i > 0; i -= 10 ) {
        sequence.a( i,0,0 );
    }
    saveSequence( 5, &sequence );
    
    sequence.clear();
    for ( int i = 0; i <= 255; i += 10 ) {
        sequence.a( 0,i,0 );
    }
    for ( int i = 255; i > 0; i -= 10 ) {
        sequence.a( 0,i,0 );
    }
    saveSequence( 6, &sequence );

    sequence.clear();
    for ( int i = 0; i <= 255; i += 10 ) {
        sequence.a( 0,0,i );
    }
    for ( int i = 255; i > 0; i -= 10 ) {
        sequence.a( 0,0,i );
    }
    saveSequence( 7, &sequence );
    
    sequence.clear().a( 0,0,0 );
    saveSequence( 8, &sequence );
 
    sequence.clear().a( 255,255,255 );
    saveSequence( 9, &sequence );

    setSequence( 0 );

    dump();
}

void Controller::loop()
{
    if (sequenceIndex == sequenceCount() ) {
        sequence.clear().append( getDialColor() );
    }

    getMode()->loop();
}


void Controller::color( byte* rgb )
{
    for ( int c = 0; c < 3; c ++ ) {
        analogWrite( rgbPins[c], 255 - rgb[c] );
    }
}

void Controller::color( byte red, byte green, byte blue )
{
    analogWrite( RED_OUT_PIN, 255 - red );
    analogWrite( GREEN_OUT_PIN, 255 - green );
    analogWrite( BLUE_OUT_PIN, 255 - blue );
}

void Controller::channel( int channel, byte value )
{
    analogWrite( rgbPins[channel], 255 - value );
}


void Controller::setMode( Mode* pMode )
{
    this->pMode = pMode;
    this->pMode->begin();
}

void Controller::setMode( int modeIndex )
{
    this->modeIndex = modeIndex;
    this->setMode( pModes[ modeIndex ] );
}

void Controller::nextMode()
{
    modeIndex ++;
    if ( modeIndex >= MODE_COUNT ) {
        modeIndex = 0;
    }
    setMode( modeIndex );
    dvalue( "Mode ", modeIndex );
}

Mode* Controller::getMode()
{
    return pMode;
}


void Controller::setEase( int index )
{
    easeIndex = index;
}

void Controller::nextEase()
{
    easeIndex ++;
    if ( easeIndex >= EASE_COUNT ) {
        easeIndex = 0;
    }
    dvalue( "Ease ", easeIndex );

}

Ease* Controller::getEase()
{
    return pEases[ easeIndex ];
}


void Controller::setSequence( int index )
{
    sequenceIndex = index;
    
    // There is a special sequence at the end, which always displays whatever colour is on the RGB dials.
    // Make a dummy sequence for it, (not loaded or saved to EEPROM).
    
    if (sequenceIndex == sequenceCount() ) {
        sequence.clear().a( 255, 255, 255 );
    } else {
        loadSequence( sequenceIndex );
    }
}

void Controller::nextSequence()
{
    sequenceIndex ++;
    if ( sequenceIndex > sequenceCount() ) {
        sequenceIndex = 0;
    }
    setSequence( sequenceIndex );
    
    getMode()->begin();
    dvalue( "Sequence ", sequenceIndex );

}



long Controller::getTickDuration()
{
    long potValue = analogRead( SPEED_PIN );
    return (potValue + 100) * potValue / 50;
}

byte tempColor[3];

byte* Controller::getDialColor()
{
    for ( int i = 0; i < 3; i ++ ) {
        tempColor[i] = getChannel( i );
    }
    return tempColor;
}

byte Controller::getChannel( int c )
{
    return analogRead(rgbInPins[c]) / 4;
}

