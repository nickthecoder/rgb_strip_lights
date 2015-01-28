#include <Arduino.h>
#include "waggle.h"

Waggle::Waggle( int pin, int period, int threshold )
{
    this->pin = pin;
    this->period = period;
    this->threshold = threshold;

    pinMode( pin, INPUT );

    reset();
}

void Waggle::reset()
{
    lastReset = millis();
    minVal = analogRead( pin );
    maxVal = minVal;
}

boolean Waggle::isWaggled()
{
    int value = analogRead( pin );
    
    if ( value < minVal ) {
        minVal = value;
    }
    
    if ( value > maxVal ) {
        maxVal = value;
    }
    
    int diff = maxVal - minVal;
    if ( diff > threshold ) {
        reset();
        return true;
    }
    
    long now = millis();
    if ( now > lastReset + period ) {
        reset();
    }
    
    return false;
}


