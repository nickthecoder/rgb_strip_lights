#ifndef HEADER_WAGGLE
#define HEADER_WAGGLE

/*
Reads the values on an analog pin, and if the values have changed significantly, then it is deemed to have been "waggled".
This was designed to be used for a noisy potentiometer (meaning that changes to the signal does not mean human intervention),
and where I want to know when a human has turned the knob.
*/

class Waggle
{

  public :
    Waggle( int pin, int period = 100, int threshold = 20 );
    
    boolean isWaggled();
    void reset();

  public :
    long period;
    int threshold;
    
  private :
    int pin;
    int minVal;
    int maxVal;
    long lastReset;
  
};

#endif

