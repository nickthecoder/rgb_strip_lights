#ifndef HEADER_SEQUENCE
#define HEADER_SEQUENCE

#include <Arduino.h>

#define MAX_SEQUENCE_LENGTH 20

extern byte BLACK[];
extern byte WHITE[];

class Sequence
{

  public :
    int length;
    byte colors[ MAX_SEQUENCE_LENGTH ][3] ;
  
  public :
    Sequence();
    
    void setColor( int index, byte rgb[] );
    void add( int afterIndex, byte rgb[] );
    void append( byte rgb[] );
    
    void deleteColor( int index );
    
    Sequence& clear();
    Sequence& a( byte r, byte g, byte b );    
};


#endif

