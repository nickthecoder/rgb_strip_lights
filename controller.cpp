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
Twinkle twinkle = Twinkle();
Alternate alternate = Alternate();
AlternateLast alternateLast = AlternateLast();
StaticMode staticMode = StaticMode();

#define MODE_COUNT 8
Mode* pModes[MODE_COUNT] = { &fade, &heartBeat, &fadeout, &whiteout, &twinkle, &alternate, &alternateLast, &staticMode };
int getModeCount() {
    return MODE_COUNT;
}

#define EASE_COUNT 6
Ease* pEases[EASE_COUNT] = {  &jump, &linear, &easeInQuad, &easeOutQuad, &easeInQuart,  &easeOutQuart };
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
RemoteInput previousSequenceInput = RemoteInput( REMOTE_JUMP3 );

Button modeButton = Button( MODE_PIN );
RemoteInput modeRemoteInput = RemoteInput( REMOTE_FLASH ); 
DualInput modeInput = DualInput( &modeButton, &modeRemoteInput );

Button easeButton = Button( EASE_PIN );
RemoteInput easeRemoteInput = RemoteInput( REMOTE_FADE7 );
DualInput easeInput = DualInput( &easeButton, &easeRemoteInput );
RemoteInput previousEaseInput = RemoteInput( REMOTE_FADE3 );

RemoteInput powerInput =  RemoteInput( REMOTE_POWER );

DualInput cancelInput = DualInput( &modeInput, &powerInput );

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);


int freeRam()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

Controller::Controller()
    : remote( Remote( REMOTE_PIN ) )
{
    pModeInput = &modeInput;
    pSequenceInput = &sequenceInput;
    pPreviousSequenceInput = &previousSequenceInput;
    pEaseInput = &easeInput;
    pPreviousEaseInput = &previousEaseInput;
    
    pAddInput = &addInput;
    pDeleteInput = &deleteInput;
    pEditInput = &editInput;
    pCancelInput = &cancelInput;
    
    modeIndex = 0;
    easeIndex = 0; //EASE_COUNT - 1;
    sequenceIndex = 0;
    brightness = 1;

    speed = 10;

    sequence.a( 255,255,255 );
}

void Controller::setup()
{
    pinMode( SPEED_PIN, INPUT );
    
    pinMode( RED_OUT_PIN, OUTPUT );
    pinMode( GREEN_OUT_PIN, OUTPUT );
    pinMode( BLUE_OUT_PIN, OUTPUT );
    pinMode( BUZZER_PIN, OUTPUT );

    u8g.setColorIndex(1);         // pixel on
    needsRedraw = true;
    u8g.setFont(u8g_font_unifont);

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


void Controller::reset()
{
    controller.beep();
    controller.speed = 10;
    controller.setMode( 0 );
    controller.setSequence( 0 );
    controller.setEase( 0 );
}
    
void Controller::resetData()
{
    save();
    reset();

    sequence.clear().a(0xff0000).a(0x00ff00).a(0x0000ff);
    saveSequence( 0, &sequence );

    sequence.clear().a(0xff0000).a(0xff4400).a(0xffaa00).a(0x00aa00).a(0x00aaff).a(0x0000ff).a(0xff00ff);
    saveSequence( 1, &sequence );

    sequence.clear().a(0x00ff00).a(0xffff00).a(0x0000ff).a(0xff00ff).a(0x00ffff).a(0xffffff).a(0xff0000);
    saveSequence( 2, &sequence );

    sequence.clear().a(0x0000ff).a(0xffffff).a(0xff0000).a(0xffffff);
    saveSequence( 3, &sequence );
    
    sequence.clear().a(0x4444ff).a(0xffffff);
    saveSequence( 4, &sequence );

    // Shades of red
    sequence.clear();
    for ( int i = 1; i <= 10; i ++ ) {
        float v = (float( i * i )) / 100;
        int c = v * 255;
        sequence.a( c,0,0 );
    }
    for ( int i = 9; i > 1; i -- ) {
        float v = (float( i * i )) / 100;
        int c = v * 255;
        sequence.a( c,0,0 );
    }
    saveSequence( 5, &sequence );
    
    // Shades of green
    sequence.clear();
    for ( int i = 1; i <= 10; i ++ ) {
        float v = (float( i * i )) / 100;
        int c = v * 255;
        sequence.a( 0,c,0 );
    }
    for ( int i = 9; i > 1; i -- ) {
        float v = (float( i * i )) / 100;
        int c = v * 255;
        sequence.a( 0,c,0 );
    }
    saveSequence( 6, &sequence );

    // Shades of blue
    sequence.clear();
    for ( int i = 1; i <= 10; i ++ ) {
        float v = (float( i * i )) / 100;
        int c = v * 255;
        sequence.a( 0,0,c );
    }
    for ( int i = 9; i > 1; i -- ) {
        float v = (float( i * i )) / 100;
        int c = v * 255;
        sequence.a( 0,0,c );
    }
    saveSequence( 7, &sequence );

    // White
    sequence.clear().a( 0xffffff );
    saveSequence( 8, &sequence );

    setSequence( 0 );

    dump();
}

void Controller::loop()
{
    long button = controller.remote.getButton();
    if ( button == REMOTE_BRIGHTNESS_UP ) {
        brightness += 0.1;
        if ( brightness > 1 ) {
            brightness = 1;
        }
        toneIndex( brightness * 10 );
        updateScreen();
    }
    if ( button == REMOTE_BRIGHTNESS_DOWN ) {
        brightness -= 0.1;
        if ( brightness < 0 ) {
            brightness = 0;
        }
        toneIndex( brightness * 10 );
        updateScreen();
    }
    
    if ( button == REMOTE_QUICK ) {
        speedUp();
    }
    
    if ( button == REMOTE_SLOW ) {
        slowDown();
    }

    remote.loop();
    if (sequenceIndex == sequenceCount() ) {
        sequence.clear().append( getDialColor() );
    }

    getMode()->loop();
    
    // Rredraw the OLED screen if needed.
    // This is SLOW (up to 0.25 seconds), so only do it when needed
    if (needsRedraw) {
        u8g.firstPage();  
        do {
            pMode->drawScreen();
        } while( u8g.nextPage() );
        needsRedraw = false;   
    }
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
    analogWrite( rgbPins[channel], 255 - (int) (brightness * (float) value) );
}


void Controller::setMode( Mode* pMode )
{
    this->pMode = pMode;
    this->pMode->begin();
    needsRedraw = true;
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
    toneIndex( modeIndex );
    dvalue( "Mode ", modeIndex );
}

Mode* Controller::getMode()
{
    return pMode;
}


void Controller::setEase( int index )
{
    easeIndex = index;
    needsRedraw = true;
}

void Controller::nextEase()
{
    easeIndex ++;
    if ( easeIndex >= EASE_COUNT ) {
        easeIndex = 0;
    }
 
    toneIndex( easeIndex );
    needsRedraw = true;
    dvalue( "Ease ", easeIndex );

}

void Controller::previousEase()
{
    easeIndex --;
    if ( easeIndex < 0 ) {
        easeIndex = EASE_COUNT - 1;
    }
    toneIndex( easeIndex );
    needsRedraw = true;
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
    needsRedraw = true;
}

void Controller::nextSequence()
{
    sequenceIndex ++;
    if ( sequenceIndex >= sequenceCount() ) {
        sequenceIndex = 0;
    }
    setSequence( sequenceIndex );

    getMode()->begin();
    toneIndex( sequenceIndex );
    dvalue( "Sequence ", sequenceIndex );
}


void Controller::previousSequence()
{
    sequenceIndex --;
    if ( sequenceIndex < 0 ) {
        sequenceIndex = sequenceCount() - 1;
    }
    setSequence( sequenceIndex );

    getMode()->begin();
    toneIndex( sequenceIndex );
    dvalue( "Sequence ", sequenceIndex );
}

Waggle speedWaggle = Waggle( SPEED_PIN );

Waggle twinkleWaggle = Waggle( TWINKLE_PIN );

// Returns a value in ms of the duration of a tick.
// Range 100..
long Controller::getTickDuration()
{
    if (twinkleWaggle.isWaggled()) {
        updateScreen();
    }
    
    if ( speed <= 0 ) {
        long potValue = 1024 - analogRead( SPEED_PIN );
        long value = 100 + (potValue * potValue) / 50;
        if ( speedWaggle.isWaggled() ) {
            updateScreen();
        }
        return value;
    } else {
        if ( speedWaggle.isWaggled() ) {
            updateScreen();
            speed = -1;
            return getTickDuration();
        }
        return speed * speed * 10;
    }
}

void Controller::slowDown()
{
    if ( speed <= 0 ) {
        speed = pow( getTickDuration(), 0.5 );
    }
    
    speed += 1;
    toneIndex( 15 -(speed % 15) ); 
    updateScreen();   
}

void Controller::speedUp()
{
    if ( speed <= 0 ) {
        speed = pow( getTickDuration(), 0.5 );
    }

    speed -= 1;
    if ( speed < 1 ) {
        speed = 1;
    }
    toneIndex( 15 - (speed % 15) ); 
    updateScreen();   
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

int toneScale[] = { TONE_C3, TONE_D3, TONE_E3, TONE_F3, TONE_G3, TONE_A3, TONE_B3, TONE_C4, TONE_D4, TONE_E4, TONE_F4, TONE_G4, TONE_A4, TONE_B4, TONE_C5 };

void Controller::toneIndex( int index, long millis )
{
    if ( index < 0 ) {
        index = 0;
    }
    if ( index > 14 ) {
        index = 14;
    }
    dvalue( "toneIndex ", index );
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

void Controller::beep()
{
    tone( TONE_C6, 50 );
}

void Controller::updateScreen()
{
    needsRedraw = true;
}


