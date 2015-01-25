#include <Arduino.h>
#include <EEPROM.h>
#include "controller.h"

int eepromAddress = 0;

void seek( int address )
{
    eepromAddress = address;
}

int readByte()
{
    return EEPROM.read( eepromAddress ++ );
}
void writeByte( int value )
{
    EEPROM.write( eepromAddress ++, value );
}

int readWord()
{
    return EEPROM.read( eepromAddress ++ ) + 256 * EEPROM.read( eepromAddress ++ );
}

int writeWord( int value )
{
    EEPROM.write( eepromAddress ++, value % 256 );
    EEPROM.write( eepromAddress ++, value / 256 );
}

int readWord( int from )
{
    return EEPROM.read( from ) + 256 * EEPROM.read( from + 1 );
}

int writeWord( int from, int value )
{
    EEPROM.write( from, value % 256 );
    EEPROM.write( from + 1, value / 256 );
}

#define MAX_SEQUENCES 20

#define L_HEAD 0
#define L_MAX_SEQUENCES 1
#define L_MODE 2
#define L_EASE 3
#define L_SEQUENCE 4
#define L_SEQUENCE_COUNT 5

#define L_SEQUENCE_TABLE 10
#define L_SEQUENCE_DATA (L_SEQUENCE_TABLE + MAX_SEQUENCES * 2)

int sequenceCount()
{
    return EEPROM.read( L_SEQUENCE_COUNT );
}

// Load data from the EEPROM
boolean load()
{
    Serial.println( "Load" );

    int mode = EEPROM.read( L_MODE );
    int ease = EEPROM.read( L_EASE );
    int seq  = EEPROM.read( L_SEQUENCE );
    int maxSeq = EEPROM.read( L_MAX_SEQUENCES );

    if ( ( mode >= getModeCount() )  || ( ease >= getEaseCount() ) || ( maxSeq != MAX_SEQUENCES) ) {
        return false;
    }
    
    controller.setMode( mode );
    controller.setEase( ease );
    controller.setSequence( seq );

    return true;
}

void reset()
{
    EEPROM.write( L_SEQUENCE_COUNT, 0 );
}

void save()
{  
    Serial.println( "Save" );
  
    EEPROM.write( L_MODE, controller.modeIndex );
    EEPROM.write( L_EASE, controller.easeIndex );
    EEPROM.write( L_SEQUENCE, controller.sequenceIndex );
    EEPROM.write( L_MAX_SEQUENCES, MAX_SEQUENCES );

}


int getSequenceEnd( int index )
{
    int seqLoc = readWord( L_SEQUENCE_TABLE + index * 2 );
    int colorCount = EEPROM.read( seqLoc );
    
    //Serial.println("gse");
    //Serial.print( "seqLoc " );Serial.print( seqLoc ); Serial.print( " colorCount " );Serial.print( colorCount ); Serial.print( " Result = " );Serial.println(seqLoc + 1 + colorCount * 3 );
    return seqLoc + 1 + colorCount * 3;
}


int getSequenceStart( int index )
{
    if ( index == 0 ) {
        return L_SEQUENCE_DATA;
    } else {
        return getSequenceEnd( index - 1 );
    }
}


void moveUp( int index, int by )
{
    int sequenceCount = EEPROM.read( L_SEQUENCE_COUNT );
    
    int from = getSequenceStart( index );
    int to = getSequenceEnd( sequenceCount -1 );

    for ( int l = to - 1; l >= from; l -- ) {
        EEPROM.write( l + by, EEPROM.read( l ) );
    }

    for ( int s = index; s < sequenceCount; s ++ ) {
        int oldStart = readWord( L_SEQUENCE_TABLE + s * 2 );
        writeWord( L_SEQUENCE_TABLE + s * 2, oldStart + by );
    }
    
    
}
    
void moveDown( int index, int by )
{
    int sequenceCount = EEPROM.read( L_SEQUENCE_COUNT );
    
    int from = getSequenceStart( index );
    int to = getSequenceEnd( sequenceCount -1 );

    for ( int l = from; l < to; l ++ ) {
        EEPROM.write( l - by, EEPROM.read( l ) );
    }
    
    for ( int s = index; s < sequenceCount; s ++ ) {
        int oldStart = readWord( L_SEQUENCE_TABLE + s * 2 );
        writeWord( L_SEQUENCE_TABLE + s * 2, oldStart - by );
    }
}


void saveSequence( int index, Sequence* pSequence )
{
    int location =  getSequenceStart( index );

    // First, check if there are sequences after us which need moving to accomodate this sequence
    int seqCount = EEPROM.read( L_SEQUENCE_COUNT );
    if ( index < seqCount - 1 ) {
        // There are sequences after this one.
        
        int oldColCount = EEPROM.read( location );
        if ( oldColCount > pSequence->length ) {
            moveDown( index + 1, (oldColCount - pSequence->length) * 3 );
        } else if ( oldColCount < pSequence->length ) {
            moveUp( index + 1, (pSequence->length - oldColCount) * 3 );
        }
    }

    writeWord( L_SEQUENCE_TABLE + index * 2, location );
    seek( location );
    writeByte( pSequence -> length );
    for ( int i = 0; i < pSequence->length; i ++ ) {

        for ( int c = 0; c < 3; c ++ ) {
            writeByte( pSequence->colors[i][c] );
        }
    }
    
    if ( EEPROM.read( L_SEQUENCE_COUNT ) < index + 1 ) {
        EEPROM.write( L_SEQUENCE_COUNT, index + 1 );
    }

}

void loadSequence( int index )
{
    int location =  getSequenceStart( index );

    controller.sequence.clear();
    
    seek( location );
    int count = readByte();
    for ( int i = 0; i < count; i ++ ) {
        controller.sequence.a( readByte(), readByte(), readByte() );
    }
}


void dump( int from, int to )
{
    Serial.print( "From " ); Serial.println( from );
    for ( int i = from; i < to; i ++ ) {
        Serial.print( EEPROM.read( i ) );
        Serial.print( " " );
        if (i % 10 == 9) {
            Serial.println();
        }
    }
    Serial.println();
    Serial.println();
}

void dump()
{
    Serial.println( "Header" );
    dump( 0, 6 );
  
    Serial.println( "Sequence Table" );
    dump( L_SEQUENCE_TABLE, L_SEQUENCE_TABLE + EEPROM.read(L_SEQUENCE_COUNT) * 2 );

    Serial.println( "Sequences" );
    for ( int i = 0; i < EEPROM.read(L_SEQUENCE_COUNT); i ++ ) {
        Serial.print( "Sequence " ); Serial.println( i );
        Serial.println( getSequenceStart( i ) );
        Serial.println( getSequenceEnd( i ) );
        dump( getSequenceStart( i ), getSequenceEnd( i ) );
    }
}

