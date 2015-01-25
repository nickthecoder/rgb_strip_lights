#ifndef HEADER_MODE
#define HEADER_MODE


class Mode {

  protected :
    long tickStart;
    
  public :
  
    virtual void begin();
    virtual void loop();

    virtual void display( float subTicks ) = 0;
    virtual long getTickDuration() = 0;
    virtual void nextTick() = 0;


};


class SequenceMode : public Mode
{
  public :
    int colorIndex;
    int previousColorIndex;
    
  public :
    virtual void begin();
    virtual void loop();

    virtual void nextTick();
    virtual long getTickDuration();

    virtual byte* getColor();
    virtual byte* getPreviousColor();

};


#endif

