#include "debug.h"
#include "sequence.h"

extern byte BLACK[] = { 0,0,0 };
extern byte WHITE[] = { 255,255,255 };
extern byte RED[] = { 255,0,0 };
extern byte GREEN[] = { 0,255,0 };
extern byte BLUE[] = { 0,0,255 };


Sequence::Sequence()
{
    length = 0;
}

Sequence& Sequence::clear()
{
    length = 0;
    return *this;
}

Sequence& Sequence::a( byte r, byte g, byte b )
{
    byte color[] = {r, g, b};
    append( color );
    return *this;
}

Sequence& Sequence::a( long colorVal )
{
    byte red = colorVal >> 16;
    byte green = (colorVal >> 8) & 0xFF;
    byte blue = colorVal & 0xFF;
    return a(red, green, blue );
}


void Sequence::setColor( int index, byte rgb[] )
{
   for ( int c = 0; c < 3; c ++ ) {
        colors[index][c] = rgb[c];
    }
}

void Sequence::append( byte rgb[] )
{
    add( length - 1, rgb );
}

void Sequence::add( int afterIndex, byte rgb[] )
{
    if ( length >= MAX_SEQUENCE_LENGTH ) {
        return;
    }

    for ( int i = length; i > afterIndex; i -- ) {
        for ( int c = 0; c < 3; c ++ ) {
            colors[i][c] = colors[i-1][c];
        }
    }
    
    for ( int c = 0; c < 3; c ++ ) {
        colors[length][c] = rgb[c];
    }
    length ++;
}

void Sequence::deleteColor( int index )
{
    for ( int i = length-1; i > index; i -- ) {
       for ( int c = 0; c < 3; c ++ ) {
            colors[i - 1][c] = colors[i][c];
        }
    }
    length --;
}

