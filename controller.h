#ifndef HEADER_CONTROLLER
#define HEADER_CONTROLLER

#include "debug.h"
#include "pins.h"
#include "button.h"
#include "ease.h"
#include "sequence.h"
#include "mode.h"
#include "standardModes.h"
#include "data.h"
#include "remote.h"


extern int getEaseCount();
extern int getModeCount();

class Controller
{
  public :
    int modeIndex;
    int easeIndex;
    int sequenceIndex;
    Mode* pMode;

    Sequence sequence;
    
    Button modeButton;
    Button sequenceButton;
    Button easeButton;
    
    Button editButton;
    Button addButton;
    Button deleteButton;
    
    Remote remote;

  public :
    Controller();

  public :
    
    void setup();
    void loop();
    
    void color( byte red, byte green, byte blue );
    void color( byte* rgb );
    void channel( int channel, byte value );

    long getTickDuration();

    void setMode( int modeIndex );
    void setMode( Mode* pMode );
    void nextMode();
    Mode* getMode();

    void setEase( int easeIndex );
    void nextEase();
    Ease* getEase();
    
    void setSequence( int sequenceIndex );
    void nextSequence();
    
    byte getChannel( int channel );
    byte* getDialColor();

  private :
    void resetData();
};

extern Controller controller;

extern StaticMode staticMode;

#endif

