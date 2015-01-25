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

class Dialout : public SequenceMode
{
  public :
    virtual void display( float subTick );
};

class Multiple : public SequenceMode
{
  private :
    int times;
    SequenceMode* pRepeatedMode;
    
  public :
    Multiple( SequenceMode *pRepeatedMode, int times );
    virtual void nextTick();
    virtual void display( float subTick );
};

class FixedPeriod : public SequenceMode
{
  public :
    FixedPeriod( long period );
    virtual void begin();
    virtual void display( float subTick );

    virtual void displaySubPeriod( float subPeriod ) = 0;
    
  private :
    long lastTime;
    long period;
};

class Twinkle : public FixedPeriod
{
  public :
    Twinkle( long period );
    virtual void displaySubPeriod( float subPeriod );
    
};

class Alternate : public FixedPeriod
{
  public :
    Alternate( long period );
    virtual void displaySubPeriod( float subPeriod );
    
};



#endif
