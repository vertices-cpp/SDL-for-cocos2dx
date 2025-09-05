
#include "Header.h"
#include "OGGeometry.h"

//#include <algorithm>
// #include <cmath>
// #include "ccMacros.h"

// implementation of Vec2
OG_BEGIN

// implementation of Size

Size::Size() : width(0), height(0)
{
}

Size::Size(float w, float h) : width(w), height(h)
{
}

Size::Size(const Size& other) : width(other.width), height(other.height)
{
}

Size::Size(const Vec2& point) : width(point.x), height(point.y)
{
}

Size& Size::operator= (const Size& other)
{
    setSize(other.width, other.height);
    return *this;
}

Size& Size::operator= (const Vec2& point)
{
    setSize(point.x, point.y);
    return *this;
}

Size Size::operator+(const Size& right) const
{
    return Size(this->width + right.width, this->height + right.height);
}

Size Size::operator-(const Size& right) const
{
    return Size(this->width - right.width, this->height - right.height);
}

Size Size::operator*(float a) const
{
    return Size(this->width * a, this->height * a);
}
Size Size::operator*(const Size &rhs) const
{
	return Size(this->width * rhs.width, this->height * rhs.height);
}
Size Size::operator*(const Vec2 &rhs) const
{
	return Size(this->width * rhs.x, this->height * rhs.y);
}
Size Size::operator/(float a) const
{
//	CCASSERT(a!=0, "CCSize division by 0.");
	assert(a != 0);
    return Size(this->width / a, this->height / a);
}
Size Size::operator/(const Size &rhs) const
{
	//	CCASSERT(a!=0, "CCSize division by 0.");
	assert(rhs.width != 0 && rhs.height!=0);
	return Size(this->width / rhs.width, this->height / rhs.height);
}

void Size::setSize(float w, float h)
{
    this->width = w;
    this->height = h;
}

bool Size::equals(const Size& target) const
{
    return (std::abs(this->width  - target.width)  < FLT_EPSILON)
        && (std::abs(this->height - target.height) < FLT_EPSILON);
}

const Size Size::ZERO = Size(0, 0);

// implementation of Rect

Rect::Rect()
{
    setRect(0.0f, 0.0f, 0.0f, 0.0f);
}

Rect::Rect(float x, float y, float width, float height)
{
    setRect(x, y, width, height);
}
Rect::Rect(const Vec2& pos, const Size& dimension)
{
    setRect(pos.x, pos.y, dimension.width, dimension.height);
}

Rect::Rect(const Rect& other)
{
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
}

Rect& Rect::operator= (const Rect& other)
{
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
    return *this;
}

// Rect & Rect::operator=(const Vec2 & other)
// {
// 	setRect(other.x, other.y, size.width,  size.height);
//	return *this;
//
void Rect::setRect(float x, float y, float width, float height)
{
    // CGRect can support width<0 or height<0
    ////CCASSERT(width >= 0.0f && height >= 0.0f, "width and height of Rect must not less than 0.");

    origin.x = x;
    origin.y = y;

    size.width = width;
    size.height = height;
}

bool Rect::equals(const Rect& rect) const
{
    return (origin.equals(rect.origin) && 
            size.equals(rect.size));
}

float Rect::getMaxX() const
{
    return origin.x + size.width;
}

float Rect::getMidX() const
{
    return origin.x + size.width / 2.0f;
}

float Rect::getMinX() const
{
    return origin.x;
}

float Rect::getMaxY() const
{
    return origin.y + size.height;
}

float Rect::getMidY() const
{
    return origin.y + size.height / 2.0f;
}

float Rect::getMinY() const
{
    return origin.y;
}

bool Rect::containsPoint(const Vec2& point) const
{
    bool bRet = false;

    if (point.x >= getMinX() && point.x <= getMaxX()
        && point.y >= getMinY() && point.y <= getMaxY())
    {
        bRet = true;
    }

    return bRet;
}

bool Rect::intersectsRect(const Rect& rect) const
{
    return !(     getMaxX() < rect.getMinX() ||
             rect.getMaxX() <      getMinX() ||
                  getMaxY() < rect.getMinY() ||
             rect.getMaxY() <      getMinY());
}

bool Rect::intersectsCircle(const Vec2& center, float radius) const
{
    Vec2 rectangleCenter((origin.x + size.width / 2),
                         (origin.y + size.height / 2));
    
    float w = size.width / 2;
    float h = size.height / 2;
    
    float dx = std::abs(center.x - rectangleCenter.x);
    float dy = std::abs(center.y - rectangleCenter.y);
    
    if (dx > (radius + w) || dy > (radius + h))
    {
        return false;
    }
    
    Vec2 circleDistance(std::abs(center.x - origin.x - w),
                        std::abs(center.y - origin.y - h));
    
    if (circleDistance.x <= (w))
    {
        return true;
    }
    
    if (circleDistance.y <= (h))
    {
        return true;
    }
    
    float cornerDistanceSq = powf(circleDistance.x - w, 2) + powf(circleDistance.y - h, 2);
    
    return (cornerDistanceSq <= (powf(radius, 2)));
}

void Rect::merge(const Rect& rect)
{
    float minX = std::min(getMinX(), rect.getMinX());
    float minY = std::min(getMinY(), rect.getMinY());
    float maxX = std::max(getMaxX(), rect.getMaxX());
    float maxY = std::max(getMaxY(), rect.getMaxY());
    setRect(minX, minY, maxX - minX, maxY - minY);
}

Rect Rect::operator*(const Vec2 & scale)
{
	return Rect(origin * scale, size * scale);
}

Rect Rect::unionWithRect(const Rect & rect) const
{
    float thisLeftX = origin.x;
    float thisRightX = origin.x + size.width;
    float thisTopY = origin.y + size.height;
    float thisBottomY = origin.y;
    
    if (thisRightX < thisLeftX)
    {
        std::swap(thisRightX, thisLeftX);   // This rect has negative width
    }
    
    if (thisTopY < thisBottomY)
    {
        std::swap(thisTopY, thisBottomY);   // This rect has negative height
    }
    
    float otherLeftX = rect.origin.x;
    float otherRightX = rect.origin.x + rect.size.width;
    float otherTopY = rect.origin.y + rect.size.height;
    float otherBottomY = rect.origin.y;
    
    if (otherRightX < otherLeftX)
    {
        std::swap(otherRightX, otherLeftX);   // Other rect has negative width
    }
    
    if (otherTopY < otherBottomY)
    {
        std::swap(otherTopY, otherBottomY);   // Other rect has negative height
    }
    
    float combinedLeftX = std::min(thisLeftX, otherLeftX);
    float combinedRightX = std::max(thisRightX, otherRightX);
    float combinedTopY = std::max(thisTopY, otherTopY);
    float combinedBottomY = std::min(thisBottomY, otherBottomY);
    
    return Rect(combinedLeftX, combinedBottomY, combinedRightX - combinedLeftX, combinedTopY - combinedBottomY);
}

const Rect Rect::ZERO = Rect(0, 0, 0, 0);

OG_END
