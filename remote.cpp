#include <Arduino.h>
#include "remote.h"
#include "controller.h"

Remote::Remote( int pin )
  : receiver(IRrecv(pin))
{
}

void Remote::setup()
{
  dprintln( "IR Setup" );
  receiver.enableIRIn(); // Start the receiver
}

decode_results results;
long button = 0;

void Remote::loop()
{
    boolean result = receiver.decode(&results);
    if (result) {
        dhexvalue( "Remote : ", results.value);
        button = results.value;
      
        receiver.resume(); // Receive the next value
    } else {
        button = 0;
    }
}

long Remote::getButton()
{
    return button;
}

