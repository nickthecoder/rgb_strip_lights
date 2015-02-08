#ifndef HEADER_STANDARD_MODES
#define HEADER_STANDARD_MODES

#include "mode.h"
#include "sequence.h"

class Fade : public EasingSequenceMode
{
  public :
    Fade() : EasingSequenceMode( F("Fade") ) {};
    virtual void display( float subTick );
};

class Fadeout : public EasingSequenceMode
{
  public :
    Fadeout() : EasingSequenceMode( F("Fadeout") ) {};
    virtual void display( float subTick );
};


class Whiteout : public EasingSequenceMode
{
  public :
    Whiteout() : EasingSequenceMode( F("Whiteout") ) {};
    virtual void display( float subTick );
};

class HeartBeat : public EasingSequenceMode
{
  public :
    HeartBeat() : EasingSequenceMode( F("Heartbeat") ) {};
    virtual void display( float subTick );
};


class Periodic : public EasingSequenceMode
{
  public :
    Periodic( const __FlashStringHelper *name) : EasingSequenceMode( name ) {};

    virtual void begin();
    virtual void display( float subTick );

    virtual void displaySubPeriod( float subPeriod ) = 0;
    virtual int getPeriod();
    
    virtual void drawScreen();
    
  private :
    long lastTime;
};

class Twinkle : public Periodic
{
  public :
    Twinkle() : Periodic( F("Twinkle") ) { this->pColor = BLACK; };
    Twinkle( const __FlashStringHelper *name, byte* pColor ) : Periodic( name ) { this->pColor = pColor; };

    virtual void displaySubPeriod( float subPeriod );
    
  public :
    byte* pColor;
};

class Alternate : public Periodic
{
  public :
    Alternate() : Periodic( F("Alternate") ) {};
    virtual void displaySubPeriod( float subPeriod );
    
};

class StaticMode : public SequenceMode
{
  public :
    StaticMode() : SequenceMode( F("Dial-a-colour") ) {};
    virtual void display( float subTick );
};

class StayMode : public SequenceMode
{
  public :
    StayMode() : SequenceMode( F("Remote Pick") ) {};
    virtual void display( float subTick );
};
extern StayMode stayMode;



#endif

