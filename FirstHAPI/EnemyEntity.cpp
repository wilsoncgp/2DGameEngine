#include "EnemyEntity.h"
#include "Visualisation.h"

// Default Constructor - Not used
CEnemyEntity::CEnemyEntity(void)
{
}

// Main constructor - Uses parent's main constructor to assign the
// values for the Graphics ID, starting position, image frame size,
// health and damage properties, whilst also setting the enemy's
// deadY and maxBullets attributes.
//
// Assigns its speed and side at creation. Also
// assigns its own Collision Rectangle, which was
// causing problems due to size of graphic used
// for this project
CEnemyEntity::CEnemyEntity(int graphicsID, int posX, int posY, int frameX, int frameY, int health, int damage, int maxBullets, int deadY)
	: CEntity(graphicsID, posX, posY, frameX, frameY, health, damage), m_deadY(deadY)
{
	m_speedX = 0; m_speedY = 3;
	m_side = Side::enemy;
	m_theta = 0.0f;
	m_timeToShoot = 0.0f;
	m_actualX = posX;
	m_actualY = posY;
	m_maxBullets = maxBullets;
	m_bullets = maxBullets;
	m_emptyClip = false;
}

// Default destructor
CEnemyEntity::~CEnemyEntity(void)
{
}

// Update Method - Updates position based on using a technique to create
// a figure of 8 motion from its centre point (m_actualX, m_actualY),
// using the theta parameter.
// Also determines if the enemy is out of the bounds by checking its current
// Y position to see if it is below the deadY value and destroys it if so.
// Finally, determines if the enemy can shoot this update.
void CEnemyEntity::Update(HAPI_TKeyboardData, HAPI_TControllerData)
{
	// For lerping purposes, set previous position to be current at
	// start of update and re-set clamp to be false
	m_prevPosX = m_posX;
	m_prevPosY = m_posY;
	m_clampS = false;

	// Increment the theta and timeToShoot variables
	m_theta += 0.05f;
	m_timeToShoot += 0.05f;

	// Use theta to create a figure of 8 motion around the centre
	// point (actualX, actualY), and increase the actualY variable
	// by its speed
	m_posX = (int)(100 * sin(m_theta)) + m_actualX;
	m_posY = (int)(100 * sin(m_theta * 2)) + m_actualY;
	m_actualY += m_speedY;

	// Translate the collision rectangle based on new positions
	m_collisionRect.Translate(m_posX - m_prevPosX, m_posY - m_prevPosY);

	// If the new position is ahead of the deadY position, Destroy this enemy
	if(m_posY > m_deadY)
		Destroy();

	// If the timeToShoot variable has passed the marker,
	// set the enemy to shoot
	if(m_timeToShoot >= 0.5f && m_posX >= 0 && m_posX <= 480)
	{
		m_hasShot = true;
		m_timeToShoot = 0.0f;
	}
	// Set to false if otherwise, or enemy will continue to shoot
	else
	{
		m_hasShot = false;
	}
}

// Rendering Method - All Drawing to screen occurs here,
// using the passed through Visualisation and lerp value 's'
void CEnemyEntity::Render(CVisualisation *vis, float s)
{
	// Set clamp to be true if s is greater than 1
	if (s >= 1.0f)
	{
		m_clampS = true;
	}

	// If clamp has been set to true, image must be drawn at
	// actual position
	if (m_clampS)
		vis->DrawTextureAnim(m_graphicsID, m_posX, m_posY);

	// If the lerp value is less than or equal to 0,
	// draw it at the previous position (likely only for
	// one update)
	else if (s <= 0 && !m_clampS)
		vis->DrawTextureAnim(m_graphicsID, m_prevPosX, m_prevPosY);

	// Else if the clamp is false alone, work out the position
	// based on the lerp value multiplied by the difference
	// in old and new positions, added onto the old position
	else if (!m_clampS)
		vis->DrawTextureAnim(m_graphicsID, (int)(m_prevPosX + (s * (m_posX - m_prevPosX))), (int)(m_prevPosY + (s * (m_posY - m_prevPosY))));

	// Test code to draw collision rectangle for this object
	//vis->DrawCollisionRectangle(GetCollisionRectangle());
}

// Destroy Method - Called when Enemy is out of bounds,
// or if its health has been reduced to 0.
void CEnemyEntity::Destroy()
{
	m_active = false;
}
