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
#include "buzzer.h"

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
    
    Input *pModeInput;
    Input *pSequenceInput;
    Input *pPreviousSequenceInput;
    Input *pEaseInput;
    Input *pPreviousEaseInput;
    
    Input *pEditInput;
    Input *pAddInput;
    Input *pDeleteInput;
    Input *pCancelInput;
    
    Remote remote;

  public :
    Controller();

  public :
    
    void setup();
    void loop();
    
    void showColor( long colorVal );
    
    void color( long colorVal );
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
    void previousEase();
    Ease* getEase();
    
    void setSequence( int sequenceIndex );
    void nextSequence();
    void previousSequence();
    
    byte getChannel( int channel );
    byte* getDialColor();

    void tone( int frequency, long millis = 100 );
    void toneIndex( int index, long millis = 100 );
    void beep();
    
  private :
    void resetData();
};

extern Controller controller;

extern StaticMode staticMode;

#endif

