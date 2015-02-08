#ifndef HEADER_MODE
#define HEADER_MODE

#include <U8glib.h>

class Mode {


  public :
    const __FlashStringHelper *name;

  protected :
    long tickStart;
    
  public :
    Mode( const __FlashStringHelper *name ) { this->name = name; };
  
    virtual void begin();
    virtual void loop();

    virtual void display( float subTicks ) = 0;
    virtual long getTickDuration() = 0;
    virtual void nextTick() = 0;
    virtual void drawScreen();

};


class SequenceMode : public Mode
{
  public :
    int colorIndex;
    int previousColorIndex;
    
  public :
    SequenceMode( const __FlashStringHelper *name ) : Mode( name ) {};
  
    virtual void begin();
    virtual void loop();

    virtual void nextTick();
    virtual long getTickDuration();

    virtual byte* getColor();
    virtual byte* getPreviousColor();

    virtual void drawScreen();
};

// Uses the controller's ease to transition from one colour to the next.
// The OLED display will show the ease's name
class EasingSequenceMode : public SequenceMode
{
  public :
    EasingSequenceMode( const __FlashStringHelper *name ) : SequenceMode( name ) {};
    virtual void drawScreen();
};


class OffMode : public Mode
{
  public :
    OffMode() : Mode( F("Off") ) {};
    
    virtual void loop();
    virtual void display(float);
    virtual long getTickDuration();
    virtual void nextTick();
};
extern OffMode offMode;
    

#endif

