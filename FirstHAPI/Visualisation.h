#pragma once

#include <HAPI_lib.h>
#include <vector>

// Declare already made classes, to avoid tangled headers
class CSprite;
class CRectangle;

/*
*
*	The Visualisation is a black-boxed component
*	used to draw everything in the game. All drawing
*	to the screen, happens within this class/its members.
*
*/

class CVisualisation
{
public:
	// Constructor and Destructor

	CVisualisation(void);
	~CVisualisation(void);
	
	// Method to Initialise the Screen

	bool Initialise(int sWidth, int sHeight);

	// Public Methods to be used to draw to the screen

	void ClearScreen(HAPI_TColour colour);
	bool LoadTexture(std::string fileName, int *id, int &fWidth, int &fHeight, bool framed = false, bool anim = false, const DWORD tFrame = 0);
	void DrawTexture(int id, int posX, int posY, bool alpha = true);
	void DrawTextureAnim(int id, int posX, int posY);
	void DrawTextureXAnimStrip(int id, int posX, int posY, int currentFrameX);
	void DrawTextureYAnimStrip(int id, int posX, int posY, int currentFrameY);
	void DrawScrollingBackground(int id, int posY);
	void DrawHealthBar(int id, int posX, int posY, int h);
	void RenderText(int posX, int posY, HAPI_TColour col, std::string text);
	bool SetSpriteAnimFrame(int id, int cX, int cY);
	void DrawCollisionRectangle(CRectangle rect);

private:
	// Private variables -
	// spriteVector - a vector list of the loaded sprites
	// screen - pointer to screen's top left corner
	// sWidth, sHeight - the screen's width and height
	std::vector<CSprite*> m_spriteVector;
	BYTE* m_screen;
	int m_sWidth, m_sHeight;
};

