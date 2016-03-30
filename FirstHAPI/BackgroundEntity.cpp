#include "BackgroundEntity.h"
#include "Visualisation.h"

// Default Constructor - not used
CBackgroundEntity::CBackgroundEntity(void)
{
}

// Main constructor - Uses parent's main constructor to assign the
// values for the Graphics ID, starting position, image frame size,
// health and damage properties
CBackgroundEntity::CBackgroundEntity(int graphicsID, int posX, int posY, int frameX, int frameY, int health, int damage)
	:CEntity(graphicsID, posX, posY, frameX, frameY, health, damage)
{
	m_collisionRect = CRectangle();
	m_scrolling = true;
	m_side = Side::neutral;
	m_speedX = 0;
	m_speedY = 10;
}

// Default Destructor
CBackgroundEntity::~CBackgroundEntity(void)
{
}

// Update Method - Updates the current position of the scrolling
// background, should the boolean 'm_scrolling' be true.
void CBackgroundEntity::Update(HAPI_TKeyboardData kData, HAPI_TControllerData cData)
{
	// For lerping purposes, set previous position to be current at
	// start of update and re-set clamp to be false
	m_prevPosY = m_posY;
	m_clampS = false;

	// Check private 'scrolling' variable
	if(m_scrolling)
	{
		// Decrement the position (to scroll the image up)
		m_posY -= m_speedY;

		// If the position is less than zero, re-set the image
		// to its original position at construction
		if(m_posY < 0)
		{
			m_prevPosY = 600;
			m_posY = 600 - m_speedY;
		}
	}
}

// Rendering Method - All Drawing to screen occurs here,
// using the passed through Visualisation and lerp value 's'
void CBackgroundEntity::Render(CVisualisation *vis, float s)
{
	// Set clamp to be true if s is greater than 1
	if (s >= 1.0f)
	{
		m_clampS = true;
	}

	// If clamp has been set to true, image must be drawn at
	// actual position
	if (m_clampS)
		vis->DrawScrollingBackground(m_graphicsID, m_posY);

	// If the lerp value is less than or equal to 0,
	// draw it at the previous position (likely only for
	// one update)
	else if (s <= 0 && !m_clampS)
		vis->DrawScrollingBackground(m_graphicsID, m_prevPosY);

	// Else if the clamp is false alone, work out the position
	// based on the lerp value multiplied by the difference
	// in old and new positions, added onto the old position
	else if (!m_clampS)
		vis->DrawScrollingBackground(m_graphicsID, (int)(m_prevPosY + (s * (m_posY - m_prevPosY))));
}

// Destroy Method - Shouldn't be called for this Entity,
// but still want entity to remain active should it do so
void CBackgroundEntity::Destroy()
{
	m_health = 0;
}