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
Twinkle twinkle = Twinkle( BLACK );
Twinkle whiteTwinkle = Twinkle( WHITE );
Twinkle redTwinkle = Twinkle( RED );
Twinkle greenTwinkle = Twinkle( GREEN );
Twinkle blueTwinkle = Twinkle(  BLUE );
Alternate alternate = Alternate();
StaticMode staticMode = StaticMode();

#define MODE_COUNT 11
Mode* pModes[MODE_COUNT] = { &fade, &heartBeat, &fadeout, &whiteout, &twinkle, &whiteTwinkle, &redTwinkle, &greenTwinkle, &blueTwinkle, &alternate, &staticMode };
int getModeCount() {
    return MODE_COUNT;
}

#define EASE_COUNT 8
Ease* pEases[EASE_COUNT] = {  &jump, &linear, &easeInQuad, &easeOutQuad, &easeInCubic, &easeOutCubic, &easeInQuart,  &easeOutQuart };
int getEaseCount() {
    return EASE_COUNT;
}

Sequence sequence = Sequence();

Button editButton = Button( EDIT_PIN );
RemoteInput editRemoteInput = RemoteInput( REMOTE_DIY1 );
DualInput editInput = DualInput( &editButton, &editRemoteInput );

Button addButton = Button( ADD_PIN );
RemoteInput addRemoteInput = RemoteInput( REMOTE_DIY2 );
DualInput addInput = DualInput( &addButton, &addRemoteInput );

Button deleteButton = Button( DELETE_PIN );
RemoteInput deleteRemoteInput = RemoteInput( REMOTE_DIY3 );
DualInput deleteInput = DualInput( &deleteButton, &deleteRemoteInput );

Button sequenceButton = Button( SEQUENCE_PIN );
RemoteInput sequenceRemoteInput = RemoteInput( REMOTE_JUMP7 );
DualInput sequenceInput = DualInput( &sequenceButton, &sequenceRemoteInput );

Button modeButton = Button( MODE_PIN );
RemoteInput modeRemoteInput = RemoteInput( REMOTE_FLASH ); 
DualInput modeInput = DualInput( &modeButton, &modeRemoteInput );

Button easeButton = Button( EASE_PIN );
RemoteInput easeRemoteInput = RemoteInput( REMOTE_FLASH );
DualInput easeInput = DualInput( &easeButton, &easeRemoteInput );

RemoteInput powerInput =  RemoteInput( REMOTE_POWER );

DualInput cancelInput = DualInput( &modeInput, &powerInput );


Controller::Controller()
    : remote(        Remote( REMOTE_PIN ) )
{
    pModeInput = &modeInput;
    pSequenceInput = &sequenceInput;
    pEaseInput = &easeInput;
    
    pAddInput = &addInput;
    pDeleteInput = &deleteInput;
    pEditInput = &editInput;
    pCancelInput = &cancelInput;
    
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
    pinMode( BUZZER_PIN, OUTPUT );

    remote.setup();
        
    // pinMode( BUZZER_PIN, OUTPUT );
    
    color( 5, 1, 0 ); // Amber... On your marks...

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
    remote.loop();
    if (sequenceIndex == sequenceCount() ) {
        sequence.clear().append( getDialColor() );
    }

    getMode()->loop();
}

void Controller::showColor( long colorVal )
{
    color( colorVal );    
    controller.setMode( &stayMode );
}

void Controller::color( long colorVal )
{
    byte red = colorVal >> 16;
    byte green = (colorVal >> 8) & 0xFF;
    byte blue = colorVal & 0xFF;
    color( red, green, blue );
}

void Controller::color( byte* rgb )
{
    for ( int c = 0; c < 3; c ++ ) {
        channel( c, rgb[c] );
    }
}

void Controller::color( byte red, byte green, byte blue )
{
    channel( 0, red );
    channel( 1, green );
    channel( 2, blue );
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
    
    loadSequence( sequenceIndex );
}

void Controller::nextSequence()
{
    sequenceIndex ++;
    if ( sequenceIndex >= sequenceCount() ) {
        sequenceIndex = 0;
    }
    setSequence( sequenceIndex );

    getMode()->begin();
    dvalue( "Sequence ", sequenceIndex );
}



long Controller::getTickDuration()
{
    long potValue = 1024 - analogRead( SPEED_PIN );
    long value = 100 + (potValue * potValue) / 50;
    //dvalue( "Duration ", value );
    return value;
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

int toneScale[] = { TONE_C3, TONE_D4, TONE_E4, TONE_F4, TONE_G4, TONE_A5, TONE_B5 };

void Controller::toneIndex( int index, long millis )
{
    if ( index < 0 ) {
        index = 0;
    }
    if ( index > 6 ) {
        index = 6;
    }
    tone( toneScale[index], millis );
}

void Controller::tone( int frequency, long millis )
{
    dprintln ("Beep" );
    
    int hperiod; //calculate 1/2 period in us
    long cycles, i;
    hperiod = (500000 / frequency) - 7; // subtract 7 us to make up for digitalWrite overhead - determined empirically
    // calculate cycles
    cycles = ((long)frequency * millis) / 1000; // calculate cycles

    for (i=0; i<= cycles; i++) {
        digitalWrite( BUZZER_PIN, HIGH);
        delayMicroseconds(hperiod);
        digitalWrite( BUZZER_PIN, LOW);
        delayMicroseconds(hperiod - 1); // - 1 to make up for fractional microsecond in digitaWrite overhead
    }
}

