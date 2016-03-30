#pragma once
#include "entity.h"

/*
*
*	This Entity is used in
*	'short bursts'.
*	
*	All this Entity is used for is
*	to display an explosion after
*	a ship has been destroyed.
*
*/

class CExplosionEntity :
	public CEntity
{
public:
	// Constructors and Destructor

	CExplosionEntity(void);
	CExplosionEntity(int graphicsID, int posX, int posY, int frameX, int frameY, int health, int damage, int currentFrameY, int maxFramesX);
	~CExplosionEntity(void);

	// Overloaded CEntity Methods

	void Update(HAPI_TKeyboardData, HAPI_TControllerData);
	void Render(CVisualisation *vis, float s);
	void Destroy();

private:
	// Private variables -
	// currentFrameX, currentFrameY - the current frame in the explosion
	// maxFramesX - the max amount of frames in the x direction (only handles horizontal animations right now)
	// hasFinished - flag to say if animation has finished or not
	int m_currentFrameX, m_currentFrameY;
	int m_maxFramesX;
	bool m_hasFinished;
};

