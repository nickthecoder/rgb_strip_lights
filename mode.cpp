#include "controller.h"
#include "edit.h"
#include "waggle.h"

Waggle waggle = Waggle( WAGGLE_PIN );

void Mode::begin()
{
    tickStart = millis();
}

void Mode::loop()
{
    long now = millis();
    long tickLength = getTickDuration();
    
    if ( now > tickStart + tickLength ) {
        nextTick();
        tickStart = now;
    }
    
    float subTicks = (((float) (now - tickStart))) / (float) tickLength;
    if (subTicks > 1 ) {
        subTicks = 1;
    }
    if (subTicks < 0 ) {
        subTicks = 0;
    }

    display( subTicks );
}

void Mode::drawScreen()
{
    u8g.drawStr( 0, 14, name );
    u8g.drawBox( 0, 15, controller.brightness * 128, 1 );
}


void SequenceMode::begin()
{
    dprintln( "Begin SequenceMode" );
    waggle.reset();
    Mode::begin();
    colorIndex = 0;
    previousColorIndex = controller.sequence.length - 1;
}

void PlainMode::loop()
{
    Mode::loop();

    // We can wiggle the "green" dial to switch to the "static" mode, where the RGB dials control the LEDs.
    if ( waggle.isWaggled() ) {
        //dpringln( "Waggled" );
        //controller.setMode( &staticMode );
    }

    if ( controller.pSequenceInput->keyPressed() ) {
        controller.nextSequence();
        dvalue( "Sequence ", controller.sequenceIndex );
        save();
    }
    
    if ( controller.pPreviousSequenceInput->keyPressed() ) {
        controller.previousSequence();
        dvalue( "Sequence ", controller.sequenceIndex );
        save();
    }
    
    if ( controller.pModeInput->keyPressed() ) {
        controller.nextMode();
        dvalue( "Mode ", controller.modeIndex );
        save();
    }


    if ( controller.pAddInput->keyPressed() ) {
        dprintln( "Add Sequence" );
        controller.setMode( &addSequenceMode );
    }

    
    long button = controller.remote.getButton();

    if ( button == REMOTE_POWER) {
        controller.showColor( 0x050000 );
        controller.setMode( &offMode );

    } else if ( button == REMOTE_PAUSE) {
        if ( controller.pMode == &pauseMode ) {
            controller.setMode( controller.modeIndex );
        } else {
            controller.setMode( &pauseMode );
        }

    } else if ( button == REMOTE_RED ) {
        controller.showColor( 0xff0000 );
    } else if ( button == REMOTE_GREEN ) {
        controller.showColor( 0x00ff00 );
    } else if ( button == REMOTE_BLUE ) {
        controller.showColor( 0x0000ff );
    } else if ( button == REMOTE_WHITE ) {
        controller.showColor( 0xffffff );
    } else if ( button == REMOTE_ORANGE ) {
        controller.showColor( 0xff6600 );
    } else if ( button == REMOTE_LGREEN ) {
        controller.showColor( 0xb4ff00 );
    } else if ( button == REMOTE_LBLUE ) {
        controller.showColor( 0x0093ff );
    } else if ( button == REMOTE_PINK ) {
        controller.showColor( 0xf9a860 );
    } else if ( button == REMOTE_ORANGE2 ) {
        controller.showColor( 0xffd800 );
    } else if ( button == REMOTE_CYAN ) {
        controller.showColor( 0x00ffff );
    } else if ( button == REMOTE_PURPLE ) {
        controller.showColor( 0x6700ff );
    } else if ( button == REMOTE_PINK2 ) {
        controller.showColor( 0xf9a8b0 );
    } else if ( button == REMOTE_ORANGE3 ) {
        controller.showColor( 0xffd24a );
    } else if ( button == REMOTE_TEAL ) {
        controller.showColor( 0x4393ba );
    } else if ( button == REMOTE_AUBERGINE ) {
        controller.showColor( 0xa923c5 );
    } else if ( button == REMOTE_PALE_BLUE ) {
        controller.showColor( 0xa1c6ff );
    } else if ( button == REMOTE_YELLOW ) {
        controller.showColor( 0xffff00 );
    } else if ( button == REMOTE_TEAL2 ) {
        controller.showColor( 0x3b5e72 );
    } else if ( button == REMOTE_MAUVE ) {
        controller.showColor( 0xf310b4 );
    } else if ( button == REMOTE_PALE_BLUE2 ) {
        controller.showColor( 0xa1c6ff );
    }
}

void SequenceMode::loop()
{
    PlainMode::loop();

    if ( controller.pEditInput->keyPressed() ) {
        dprintln( "Edit" );
        controller.setMode( &browseMode );
    }
  
    if ( controller.pDeleteInput->keyPressed() ) {
        dprintln( "Delete Sequence" );
        controller.setMode( &deleteSequenceMode );
    }

}

void SequenceMode::nextTick()
{
    previousColorIndex = colorIndex;
    colorIndex = colorIndex + 1;
    if (colorIndex >= controller.sequence.length) {
        colorIndex = 0;
    }
}

byte* SequenceMode::getColor()
{
    return controller.sequence.colors[ colorIndex ];
}

byte* SequenceMode::getPreviousColor()
{
    return controller.sequence.colors[ previousColorIndex ];
}

long SequenceMode::getTickDuration()
{
    return controller.getTickDuration();
}

void SequenceMode::drawScreen()
{
    Mode::drawScreen();
    
    u8g.drawStr( 0, 30, F("Sequence") );
    u8g.setPrintPos( 80,30 );
    u8g.print( controller.sequenceIndex + 1 );
    u8g.print( "/" );
    u8g.print( sequenceCount() );
}


void EasingSequenceMode::loop()
{
    SequenceMode::loop();
    
    if ( controller.pEaseInput->keyPressed() ) {
        controller.nextEase();
        dvalue( "Ease ", controller.easeIndex );
        save();
    }

    if ( controller.pPreviousEaseInput->keyPressed() ) {
        controller.previousEase();
        dvalue( "Ease ", controller.easeIndex );
        save();
    }
}

void EasingSequenceMode::drawScreen()
{
    SequenceMode::drawScreen();
    
    u8g.drawStr( 0, 45, controller.getEase()->name );

    int duration = getTickDuration();
    float x = 140.0 - pow( duration, 0.5 );
    if ( x < 0 ) {
      x = 1;
    }
    if ( x > 127 ) {
      x = 127;
    }
    u8g.drawBox( 0, 54, (int) x, 3 );
}


OffMode offMode = OffMode();

void OffMode::loop()
{
    if ( controller.pCancelInput->keyPressed() ) {
        // Reset the state, so if you get lost, Turn it off and on again ;-)
        controller.reset();
    }
}

void OffMode::display(float)
{
}
long OffMode::getTickDuration()
{
    return 100;
}
void OffMode::nextTick()
{
}


