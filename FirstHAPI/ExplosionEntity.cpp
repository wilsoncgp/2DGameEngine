#include "ExplosionEntity.h"
#include "Visualisation.h"

// Default Constructor - Not Used
CExplosionEntity::CExplosionEntity(void)
{
}

// Main constructor - Uses parent's main constructor to assign the
// values for the Graphics ID, starting position, image frame size,
// health and damage properties, whilst also setting the explosion's
// currentFrameY and maxFramesX variables.
// Also assigns value to speed, side, currentFrameX and the hasFinished flag.
CExplosionEntity::CExplosionEntity(int graphicsID, int posX, int posY, int frameX, int frameY, int health, int damage, int currentFrameY, int maxFramesX)
	:CEntity(graphicsID, posX, posY, frameX, frameY, health, damage), m_currentFrameY(currentFrameY), m_maxFramesX(maxFramesX)
{
	m_hasFinished = true;
	m_speedX = 0; m_speedY = 0;
	m_side = Side::neutral;
	m_currentFrameX = 0;
}

CExplosionEntity::~CExplosionEntity(void)
{
}

void CExplosionEntity::Update(HAPI_TKeyboardData, HAPI_TControllerData)
{
	// If it's still alive by now, set the hasFinished to be false
	m_hasFinished = false;

	// If the currentFrame is less than the maxFrames, increment it
	if (m_currentFrameX < m_maxFramesX)
	{
		m_currentFrameX++;
	}
	// If not, set the flag to be true and Destroy the Explosion
	else
	{
		m_hasFinished = true;
		Destroy();
	}
}

// Rendering Method - All Drawing to screen occurs here,
// using the passed through Visualisation and lerp value 's'
void CExplosionEntity::Render(CVisualisation *vis, float s)
{
	// If the animation hasn't finished, draw and animate the strip across
	if (!m_hasFinished)
		vis->DrawTextureYAnimStrip(m_graphicsID, m_posX, m_posY, m_currentFrameY);
}

// Destroy Method - Set to Inactive, to be deleted
void CExplosionEntity::Destroy()
{
	m_active = false;
}
