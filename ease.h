#ifndef HEADER_EASE
#define HEADER_EASE

class Ease {
  public : virtual float ease( float from ) = 0;
};



class Linear : public Ease {
  public : virtual float ease( float from );
};
extern Linear linear;

class Jump : public Ease {
  public : virtual float ease( float from );
};
extern Jump jump;



class EaseInQuad : public Ease {
  public : virtual float ease( float from );
};
extern EaseInQuad easeInQuad;

class EaseInCubic : public Ease {
  public : virtual float ease( float from );
};
extern EaseInCubic easeInCubic;

class EaseInQuart : public Ease {
  public : virtual float ease( float from );
};
extern EaseInQuart easeInQuart;




class EaseOutQuad : public Ease {
  public : virtual float ease( float from );
};
extern EaseOutQuad easeOutQuad;

class EaseOutCubic : public Ease {
  public : virtual float ease( float from );
};
extern EaseOutCubic easeOutCubic;

class EaseOutQuart : public Ease {
  public : virtual float ease( float from );
};
extern EaseOutQuart easeOutQuart;

#endif

