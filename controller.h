#ifndef HEADER_CONTROLLER
#define HEADER_CONTROLLER

#include <U8glib.h>

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
#include "waggle.h"

extern int getEaseCount();
extern int getModeCount();

extern U8GLIB_SSD1306_128X64 u8g;

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

    float brightness; // (0..1);
    boolean needsRedraw;

  public :
    Controller();

  public :
    
    void setup();
    void loop();
    
    void reset();
    
    void showColor( long colorVal );
    
    void color( long colorVal );
    void color( byte red, byte green, byte blue );
    void color( byte* rgb );
    void channel( int channel, byte value );

    long getTickDuration();
    void speedUp();
    void slowDown();

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
    void updateScreen();
    
  private :
    void resetData();
    long speed;
    
};

extern Controller controller;

extern StaticMode staticMode;

#endif

