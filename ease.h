#ifndef HEADER_EASE
#define HEADER_EASE

#include <U8glib.h>

class Ease {
  public :
    const __FlashStringHelper *name;
    virtual float ease( float from ) = 0;
    
  public :
    Ease( const __FlashStringHelper *name ) { this->name = name; };
};


class Linear : public Ease {
  public :
    Linear() : Ease( F("Linear") ) {};
    virtual float ease( float from );
};
extern Linear linear;

class Jump : public Ease {
  public :
    Jump() : Ease( F("Jump") ) {};
    virtual float ease( float from );
};
extern Jump jump;



class EaseInQuad : public Ease {
  public :
    EaseInQuad() : Ease( F("Ease-In-Quad") ) {};
    virtual float ease( float from );
};
extern EaseInQuad easeInQuad;

/*
class EaseInCubic : public Ease {
  public :
    EaseInCubic() : Ease( F("Ease-In-Cubic") ) {};
    virtual float ease( float from );
};
extern EaseInCubic easeInCubic;
*/

class EaseInQuart : public Ease {
  public :
    EaseInQuart() : Ease( F("Ease-In-Quart") ) {};
    virtual float ease( float from );
};
extern EaseInQuart easeInQuart;




class EaseOutQuad : public Ease {
  public :
    EaseOutQuad() : Ease( F("Ease-Out-Quad") ) {};
    virtual float ease( float from );
};
extern EaseOutQuad easeOutQuad;

class EaseOutCubic : public Ease {
  public :
    EaseOutCubic() : Ease( F("Ease-Out-Cubic") ) {};
    virtual float ease( float from );
};
extern EaseOutCubic easeOutCubic;

class EaseOutQuart : public Ease {
  public :
    EaseOutQuart() : Ease( F("Ease-Out-Quart") ) {};
    virtual float ease( float from );
};
extern EaseOutQuart easeOutQuart;

#endif

