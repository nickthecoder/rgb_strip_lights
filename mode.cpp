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

    if ( controller.modeButton.keyPressed() ) {
        controller.nextMode();
        dvalue( "Mode ", controller.modeIndex );
        save();
    }

    if ( controller.easeButton.keyPressed() ) {
        controller.nextEase();
        dvalue( "Ease ", controller.easeIndex );
        save();
    }

    if ( controller.sequenceButton.keyPressed() ) {
        controller.nextSequence();
        dvalue( "Sequence ", controller.sequenceIndex );
        save();
    }
    
    if ( controller.editButton.keyPressed() ) {
        dprintln( "Edit" );
        controller.setMode( &browseMode );
    }

    if ( controller.addButton.keyPressed() ) {
        dprintln( "Add Sequence" );
        controller.setMode( &addSequenceMode );
    }

    if ( controller.deleteButton.keyPressed() ) {
        dprintln( "Delete Sequence" );
        controller.setMode( &deleteSequenceMode );
    }

    controller.remote.checkButtons();
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



