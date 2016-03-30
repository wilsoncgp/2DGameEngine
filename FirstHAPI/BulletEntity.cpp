#include "BulletEntity.h"
#include "Visualisation.h"

// Default Constructor - Not Used
CBulletEntity::CBulletEntity(void)
{
}

// Main constructor - Uses parent's main constructor to assign the
// values for the Graphics ID, starting position, image frame size,
// health and damage properties
//
// Assigns its speed and side at creation. Also
// assigns its own Collision Rectangle, which was
// causing problems due to size of graphic used
// for this project
CBulletEntity::CBulletEntity(int graphicsID, int posX, int posY, int frameX, int frameY, int health, int damage, Side side)
	:CEntity(graphicsID, posX, posY, frameX, frameY, health, damage)
{
	m_collisionRect = CRectangle(posX + 1, posY + 1, posX + frameX - 1, posY + frameY - 1);
	
	m_side = side;

	m_speedX = 0;
	m_speedY = 30;
}

// Default Destructor
CBulletEntity::~CBulletEntity(void)
{
}

// Update Method - Updates its position based on
// which side it is on. i.e. if the player has shot
// this bullet, the bullet would have to be moving up
// and vice versa for an enemy.
void CBulletEntity::Update(HAPI_TKeyboardData kData, HAPI_TControllerData cData)
{
	// For lerping purposes, set previous position to be current at
	// start of update and re-set clamp to be false
	m_prevPosY = m_posY;
	m_clampS = false;

	// If the bullet came from a player, decrement the
	// bullet's position and collision rectangle by its speed.
	if (m_side == Side::player)
	{
		m_posY -= m_speedY;
		m_collisionRect.Translate(0, -m_speedY);
	}

	// Else if its an enemy's bullet, increment the
	// bullet's position and collision rectangle by its speed.
	else if (m_side == Side::enemy)
	{
		m_posY += m_speedY;
		m_collisionRect.Translate(0, m_speedY);
	}

}

// Rendering Method - All Drawing to screen occurs here,
// using the passed through Visualisation and lerp value 's'
void CBulletEntity::Render(CVisualisation *vis, float s)
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

// Destroy Method - Called when Bullet collides and its 'health'
// is reduced to zero
void CBulletEntity::Destroy()
{
	m_active = false;
}

// Setter - Sets the Side of this Bullet
void CBulletEntity::SetSide(Side side)
{
	m_side = side;
}