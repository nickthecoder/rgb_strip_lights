#include <Arduino.h>

// **** Use to turn debugging on/off
// #define DEBUG

#ifdef DEBUG
  #define dprint(x)    Serial.print (x); Serial.flush()
  #define dprintln(x)  Serial.println (x); Serial.flush()
  #define dvalue(s,v)  Serial.print( s ); Serial.println( v ); Serial.flush()
  #define dhexvalue(s,v) Serial.print( s ); Serial.println( v, HEX ); Serial.flush()
#else
  #define dprint(x)
  #define dprintln(x)
  #define dvalue(s,v) 
  #define dhexvalue(s,v) 
#endif 

