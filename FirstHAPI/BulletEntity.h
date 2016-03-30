#pragma once
#include "entity.h"

/*
*
*	Entity to be used for a base bullet class.
*	This entity handles updating its own position
*	(based on which side it is on),
*	drawing to screen based on its graphics ID
*
*/

class CBulletEntity :
	public CEntity
{
public:
	// Constructors and Destructor

	CBulletEntity(void);
	CBulletEntity(int graphicsID, int posX, int posY, int frameX, int frameY, int health, int damage, Side side);
	~CBulletEntity(void);

	// Overloaded CEntity Methods

	void Update(HAPI_TKeyboardData kData, HAPI_TControllerData cData);
	void Render(CVisualisation *vis, float s);
	void Destroy();

	// Setter
	// (For future use perhaps, 
	// if bullets should change allegiance...)
	void SetSide(Side side);
};

