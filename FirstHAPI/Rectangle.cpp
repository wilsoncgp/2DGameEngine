#include "Rectangle.h"

// Default Constructor - Sets Values to 0
CRectangle::CRectangle(void) : left(0), top(0), right(0), bottom(0)
{
}

// Main Constructor - Sets values of attributes based on parameters
CRectangle::CRectangle(int l, int t, int r, int b) : left(l), top(t), right(r), bottom(b)
{
}

// Default Destructor
CRectangle::~CRectangle(void)
{
}

// Getter - Returns Width using R - L
unsigned int CRectangle::Width() const
{
	return right - left;
}

// Getter - Returns Height using B - T
unsigned int CRectangle::Height() const
{
	return bottom - top;
}

// Translates the Rectangle based on delta x and y values passed in
void CRectangle::Translate(int dx, int dy)
{
	left += dx; right += dx;
	top += dy; bottom += dy;
}

// Clips the rectangle aginst another rectangle
void CRectangle::ClipAgainst(const CRectangle &other)
{
	if (left < other.left)
		left = other.left;
	if (right >= other.right)
		right = other.right;
	if (top < other.top)
		top = other.top;
	if (bottom >= other.bottom)
		bottom = other.bottom;
}

// Returns true if this rectangle has collided within the other
const bool CRectangle::Collide(const CRectangle &other) const
{
	if ((left <= other.right && right >= other.left) &&
		(top <= other.bottom && bottom >= other.top))
	{
		return true;
	}

	return false;
}