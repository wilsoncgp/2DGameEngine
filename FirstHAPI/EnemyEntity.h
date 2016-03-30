#pragma once
#include "entity.h"

/*
*
*	Entity to be used for a base Enemy class.
*	This Entity handles its own position
*	updating and drawing.
*	It also handles whether it has just taken
*	a shot at the player.
*
*/

class CEnemyEntity :
	public CEntity
{
public:
	// Constructors and Destructor

	CEnemyEntity(void);
	CEnemyEntity(int graphicsID, int posX, int posY, int frameX, int frameY, int health, int damage, int maxBullets, int deadY);
	~CEnemyEntity(void);

	// Overloaded CEntity methods

	void Update(HAPI_TKeyboardData kData, HAPI_TControllerData cData);
	void Render(CVisualisation *vis, float s);
	void Destroy();

private:
	// Private Variables -
	// theta - angle for Enemy AI movement
	// timeToShoot - variable incremented per tick, timing enemy shots
	// actualX, actualY - variables for position if not moving
	// deadY - variable to say when Enemy should die if not by shots
	float m_theta;
	float m_timeToShoot;
	int m_actualX, m_actualY;
	int m_deadY;
};

