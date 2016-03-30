#pragma once

/*
*
*	Class used to represent
*	a Rectangle. Used for
*	Collisions and Drawing.
*
*/

class CRectangle
{
public:
	// Constructors and Destructor
	CRectangle(void);
	CRectangle(int l, int t, int r, int b);
	~CRectangle(void);

	// Attributes of the Rectangle
	int left, right, top, bottom;

	// Methods to find Width and Height
	unsigned int Width() const;
	unsigned int Height() const;

	// Methods to Translate the Rectangle,
	// to clip it against another,
	// and to check if 2 are colliding
	void Translate(int dx, int dy);
	void ClipAgainst(const CRectangle& other);
	const bool Collide(const CRectangle& other) const;
};