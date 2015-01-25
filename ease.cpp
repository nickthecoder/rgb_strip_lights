#include "ease.h"

float Linear::ease( float from  )
{
    return from;
}
Linear linear = Linear();

float Jump::ease( float from )
{
    return from < 0.5 ? 0 : 1;
}
Jump jump = Jump();



float EaseInQuad::ease( float from )
{
    return from * from;
}
EaseInQuad easeInQuad = EaseInQuad();

float EaseInCubic::ease( float from )
{
    return from * from * from;
}
EaseInCubic easeInCubic = EaseInCubic();

float EaseInQuart::ease( float from )
{
    return from * from * from * from;
}
EaseInQuart easeInQuart = EaseInQuart();



float EaseOutQuad::ease( float from )
{
    return 1 - easeInQuad.ease(1-from);
}
EaseOutQuad easeOutQuad = EaseOutQuad();

float EaseOutCubic::ease( float from )
{
    return 1 - easeInCubic.ease(1-from);
}
EaseOutCubic easeOutCubic = EaseOutCubic();

float EaseOutQuart::ease( float from )
{
    return 1 - easeInQuart.ease(1-from);
}
EaseOutQuart easeOutQuart = EaseOutQuart();

