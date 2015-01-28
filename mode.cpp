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


void SequenceMode::begin()
{
    dprintln( "Begin SequenceMode" );
    waggle.reset();
    Mode::begin();
    colorIndex = 0;
    previousColorIndex = controller.sequence.length - 1;
}

void SequenceMode::loop()
{
    Mode::loop();
    
    // We can wiggle the "green" dial to switch to the "static" mode, where the RGB dials control the LEDs.
    if ( waggle.isWaggled() ) {
        //dpringln( "Waggled" );
        //controller.setMode( &staticMode );
    }

    if ( controller.pModeInput->keyPressed() ) {
        controller.nextMode();
        dvalue( "Mode ", controller.modeIndex );
        save();
    }

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

    if ( controller.pEditInput->keyPressed() ) {
        dprintln( "Edit" );
        controller.setMode( &browseMode );
    }

    if ( controller.pAddInput->keyPressed() ) {
        dprintln( "Add Sequence" );
        controller.setMode( &addSequenceMode );
    }

    if ( controller.pDeleteInput->keyPressed() ) {
        dprintln( "Delete Sequence" );
        controller.setMode( &deleteSequenceMode );
    }

    
    long button = controller.remote.getButton();

    if ( button == REMOTE_POWER) {
        controller.showColor( 0x050000 );

    } else if ( button == REMOTE_PAUSE) {
        if ( controller.pMode == &stayMode ) {
            controller.setMode( controller.modeIndex );
        } else {
            controller.setMode( &stayMode );
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



