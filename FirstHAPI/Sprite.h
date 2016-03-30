#pragma once

#include <HAPI_lib.h>
#include "Rectangle.h"
#include <chrono>

/*
*
*	The Sprite class holds data
*	for a sprite and offers the user
*	the opportunity to load and draw
*	this specific sprite.
*
*	The Sprite will be accessed from
*	the Visualisation Object.
*
*/

class CSprite
{
public:
	// Constructor and Destructor
	CSprite(void);
	~CSprite(void);

	// Method to Load a Texture
	bool LoadTexture(std::string fileName, bool framed, int &fWidth, int &fHeight, bool anim, const DWORD tFrame);

	// Different Methods used to Draw a Given Sprite
	void DrawTextureNonAlpha(BYTE* dest, int posX, int posY, int destWidth, int destHeight);
	void DrawTexture(BYTE* dest, int posX, int posY, int destWidth, int destHeight);
	void DrawTextureAnim(BYTE* dest, int posX, int posY, int destWidth, int destHeight);
	void DrawScrollingBackground(BYTE* dest, int posY, int destWidth, int destHeight);
	void DrawHealthBar(BYTE* dest, int posX, int posY, int destWidth, int destHeight, int h);
	void DrawTextureXAnimStrip(BYTE* dest, int posX, int posY, int destWidth, int destHeight, int currentFrameX);
	void DrawTextureYAnimStrip(BYTE* dest, int posX, int posY, int destWidth, int destHeight, int currentFrameY);

	// Used for Animation - Sets the Current Frame
	bool SetCurrentFrame(int currentX, int currentY);

private:
	// Privately Run Method to Run the Animation for an Animated sprite
	void RunAnimation();

	// Private variables -
	// data - holds the image data of the loaded image
	// width, height - the width and height of the loaded image
	// frameWidth, frameHeight - mainly for animation, the size of each sprite in an animation
	// animation - flag to say if this sprite is for animation purposes
	// currentFrameX, currentFrameY - current Frame into the animation
	// sourceRect, frameRect - rectangle of the source image and the frame of the animation
	// time - current time
	// timeBetweenFrames - the time between frames
	BYTE* m_data;
	int m_width, m_height;
	int m_frameWidth, m_frameHeight;
	int m_animation;
	int m_currentFrameX, m_currentFrameY;
	CRectangle m_sourceRect, m_frameRect;
	DWORD m_time;
	DWORD m_timeBetweenFrames;
};