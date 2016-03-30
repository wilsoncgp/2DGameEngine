#pragma once
#include "Entity.h"

/*
*
*	Probably the most important Entity,
*	the Entity for the Player!
*	
*	The PlayerEntity class handles
*	input from the keyboard and controller,
*	to update the position, whether or not
*	the player has taken a shot, the updating
*	of bullets and clamping the player inside
*	a given field of play.
*
*/

class CPlayerEntity :
	public CEntity
{
public:
	// Constructors and Destructor

	CPlayerEntity();
	CPlayerEntity(int graphicsID, int posX, int posY, int frameX, int frameY, int health, int damage, int maxBullets);
	~CPlayerEntity(void);

	// Overloaded CEntity Methods

	void Update(HAPI_TKeyboardData kData, HAPI_TControllerData cData);
	void Render(CVisualisation* vis, float s);
	void Destroy();
	
};

