
#include "OGTouch.h"

//#include "OGAutoreleasePool.h"


OG_BEGIN

// returns the current touch location in screen coordinates
Vec2 Touch::getLocationInView() const 
{ 
    return _point; 
}

// returns the previous touch location in screen coordinates
Vec2 Touch::getPreviousLocationInView() const 
{ 
    return _prevPoint; 
}

// returns the start touch location in screen coordinates
Vec2 Touch::getStartLocationInView() const 
{ 
    return _startPoint; 
}

// returns the current touch location in OpenGL coordinates
Vec2 Touch::getLocation() const
{ 
    return /*Director::getInstance()->convertToGL*/(_point); 
}

// returns the previous touch location in OpenGL coordinates
Vec2 Touch::getPreviousLocation() const
{ 
    return /*Director::getInstance()->convertToGL*/(_prevPoint);  
}

// returns the start touch location in OpenGL coordinates
Vec2 Touch::getStartLocation() const
{ 
    return /*Director::getInstance()->convertToGL*/(_startPoint);  
}

// returns the delta position between the current location and the previous location in OpenGL coordinates
Vec2 Touch::getDelta() const
{     
    return getLocation() - getPreviousLocation();
}

// Returns the current touch force for 3d touch.
float Touch::getCurrentForce() const
{
    return _curForce;
}

// Returns the maximum touch force for 3d touch.
float Touch::getMaxForce() const
{
    return _maxForce;
}




OG_END
