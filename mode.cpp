#include "controller.h"
#include "edit.h"

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
    Mode::begin();
    colorIndex = 0;
    previousColorIndex = controller.sequence.length - 1;
}

void SequenceMode::loop()
{
    Mode::loop();
    
    if ( controller.modeButton.keyPressed() ) {
        controller.nextMode();
        save();
    }

    if ( controller.easeButton.keyPressed() ) {
        controller.nextEase();
        save();
    }

    if ( controller.sequenceButton.keyPressed() ) {
        controller.nextSequence();
        save();
    }

    if ( controller.editButton.keyPressed() ) {
        controller.setMode( &browseMode );
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



