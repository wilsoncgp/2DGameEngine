#pragma once
#include "entity.h"

/*
*
*	Entity for the Background of the game.
*	This entity only controls a scrolling
*	background and draws it to the screen.
*
*/

class CBackgroundEntity :
	public CEntity
{
public:
	// Constructors and Destructor

	CBackgroundEntity(void);
	CBackgroundEntity(int graphicsID, int posX, int posY, int frameX, int frameY, int health, int damage);
	~CBackgroundEntity(void);
	
	// Overloaded CEntity methods

	void Update(HAPI_TKeyboardData kData, HAPI_TControllerData cData);
	void Render(CVisualisation *vis, float s);
	void Destroy();

private:
	// Boolean to determine whether the
	// image will scroll or not
	bool m_scrolling;
};

