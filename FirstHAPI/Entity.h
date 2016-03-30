#pragma once
#include <HAPI_lib.h>
#include "Rectangle.h"

// Declare already made classes, to avoid tangled headers
class CVisualisation;

/*
*
*	The Entity class is a base class
*	for which others can be created.
*
*	The Entity class is never used directly
*	itself, just inherited from. The original
*	decision was for this to be a completely
*	abstract class made of just a header.
*	But there was a lot that different entities
*	shared between one another, particularly the
*	need for Getters, so to save repeated code,
*	a .cpp file was also created.
*
*/

// Enumeration of the types of side an entity can be
enum Side {neutral, player, enemy};

class CEntity
{
public:
	//Constructors and Destructor

	CEntity(void);
	CEntity(int graphicsID, int posX, int posY, int frameX, int frameY, int health, int damage);
	~CEntity(void);
	
	// Virtual Methods to be used by each Entity

	virtual void Update(HAPI_TKeyboardData kData, HAPI_TControllerData cData) = 0;
	virtual void Render(CVisualisation *vis, float s) = 0;
	virtual void Destroy() = 0;

	// Getters

	const int GetPosX();
	const int GetPosY();
	const CRectangle GetCollisionRectangle();
	const int GetHealth();
	const int GetDamage();
	const Side GetSide();
	const bool IsActive();
	const bool HasShot();

	// Bullet Specific Code used across certain Entities.
	// Ideally, this Entity would be inherited from to create
	// an In-Play Entity, so that the bullet data is only relevant
	// to those In-Play Entities. That would also ensure no collision
	// checks were done with Entities such as Background/HUD Entities

	void ReloadBullet();
	const int GetBullets();
	const int GetMaxBullets();
	const bool GetEmptyClip();

	// Reduces Health of Entity

	void Damage(int d);


protected:
	// Private Variables -
	// graphicsID - an ID into the Sprite Vector of the Visualisation object
	// posX, posY - position of top-left corner of entity
	// prevPosX, prevPosY - position from the previous update, used with lerping
	// speedX, speedY - pixels to travel per tick (if used)
	// collisionRect - the rectangle used to check collisions
	// side - tells whether the side is a player, enemy or neutral entity
	// health - the health of the entity
	// damage - the amount of damage caused by colliding with this entity
	// bullets, maxBullets - the current number of and max number of bullets for this entity
	// emptyClip - flag for saying clip is empty (had problem with final bullet in clip firing)
	// active - flag to say entity should be in play; if the entity is not active, IT IS INCOMPATIBLE, IT WILL BE DELETED
	// hasShot - flag to say the entity has shot and thus a bullet should be created this tick
	// clampS - alternative to clamping the S value in lerping

	int m_graphicsID;
	int m_posX, m_posY;
	int m_prevPosX, m_prevPosY;
	int m_speedX, m_speedY;
	CRectangle m_collisionRect;
	Side m_side;
	int m_health;
	int m_damage;
	int m_bullets, m_maxBullets;
	bool m_emptyClip;
	bool m_active;
	bool m_hasShot;
	bool m_clampS;
};

