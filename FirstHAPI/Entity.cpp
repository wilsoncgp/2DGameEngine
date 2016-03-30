#include "Entity.h"

// Default Constructor - Not Used
CEntity::CEntity(void)
{

}

// Main Constructor - Assigns the values for the graphicsID, positions, collision rectangle, health and damage,
//	whilst also setting initial flag values
CEntity::CEntity(int graphicsID, int posX, int posY, int frameX, int frameY, int health, int damage)
{
	m_graphicsID = graphicsID;
	m_posX = posX; m_posY = posY;
	m_prevPosX = posX; m_prevPosY = posY;
	m_collisionRect = CRectangle(posX + 3, posY + 3, posX + frameX - 3, posY + frameY - 3);
	m_health = health;
	m_damage = damage;
	m_clampS = false;
	m_active = true;
	m_hasShot = false;
}

// Default Destructor
CEntity::~CEntity(void)
{
}

// Getter - Return the X value of the Position
const int CEntity::GetPosX()
{
	return m_posX;
}

// Getter - Return the Y value of the Position
const int CEntity::GetPosY()
{
	return m_posY;
}

// Getter - Return the Collision Rectangle for this Entity
const CRectangle CEntity::GetCollisionRectangle()
{
	return m_collisionRect;
}

// Getter - Return the current health of this Entity
const int CEntity::GetHealth()
{
	return m_health;
}

// Getter - Return the Damage rating of this Entity
const int CEntity::GetDamage()
{
	return m_damage;
}

// Getter - Return the Side in which the Entity is part of
const Side CEntity::GetSide()
{
	return m_side;
}

// Getter - Return whether or not the Entity is active or not
const bool CEntity::IsActive()
{
	return m_active;
}

// Getter - Return whether or not the Entity has taken a shot
const bool CEntity::HasShot()
{
	return m_hasShot;
}

// Increment the Bullets by 1 (not used yet)
void CEntity::ReloadBullet()
{
	m_bullets++;
}

// Getter - Return the current amount of bullets
const int CEntity::GetBullets()
{
	return m_bullets;
}

// Getter - Return the maximum amount of bullets this entity can have
const int CEntity::GetMaxBullets()
{
	return m_maxBullets;
}

// Used in Collision Checks, takes in the damage value of other Entity
// to decrement this Entity's health by it. If the health is then less
// than or equal to 0, this Entity is Destroyed/Deleted/Exterminated
void CEntity::Damage(int d)
{ 
	m_health -= d;
	if(m_health <= 0)
		Destroy();
}

// Getter - Return whether or not the clip is empty for this Entity
const bool CEntity::GetEmptyClip()
{
	return m_emptyClip;
}
