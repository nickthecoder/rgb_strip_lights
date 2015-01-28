#ifndef HEADER_STANDARD_MODES
#define HEADER_STANDARD_MODES

#include "mode.h"


class Fade : public SequenceMode
{
  public :
    virtual void display( float subTick );
};

class Fadeout : public SequenceMode
{
  public :
    virtual void display( float subTick );
};


class Whiteout : public SequenceMode
{
  public :
    virtual void display( float subTick );
};

class HeartBeat : public SequenceMode
{
  public :
    virtual void display( float subTick );
};


class Periodic : public SequenceMode
{
  public :
    virtual void begin();
    virtual void display( float subTick );

    virtual void displaySubPeriod( float subPeriod ) = 0;
    virtual int getPeriod();
    
  private :
    long lastTime;
};

class Twinkle : public Periodic
{
  public :
    Twinkle( byte* pColor );
    
    virtual void displaySubPeriod( float subPeriod );
    
  public :
    byte* pColor;
};

class Alternate : public Periodic
{
  public :
    virtual void displaySubPeriod( float subPeriod );
    
};

class StaticMode : public SequenceMode
{
  public :
    virtual void display( float subTick );
};


class StayMode : public SequenceMode
{
  public :
    virtual void display( float subTick );
};
extern StayMode stayMode;

#endif
