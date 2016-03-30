#pragma once
#include "entity.h"

/*
*
*	The HUD Entity is another
*	neutral Entity which holds
*	the data displayed in the HUD
*	at the bottom of the screen.
*	
*	It displays the current health,
*	ammo and score at the bottom of
*	the screen.
*
*/

class CHUDEntity :
	public CEntity
{
public:
	// Constructors and Destructor
	CHUDEntity(void);
	CHUDEntity(int graphicsID, int posX, int posY, int frameX, int frameY, int health, int damage,
						int healthID, int bulletID);
	~CHUDEntity(void);

	// Overloaded CEntity Methods

	void Update(HAPI_TKeyboardData kData, HAPI_TControllerData cData);
	void Render(CVisualisation *vis, float s);
	void Destroy();
	
	// Setters for HUD elements

	void SetScore(int score);
	void SetPlayerHealth(int health);
	void SetAmmo(int ammo);

private:
	// Private variables -
	// healthID, bulletID - the graphics IDs for the health and bullet textures
	// score - current score
	// pHealth - current player health
	// ammo - current player ammo
	int m_healthID, m_bulletID;
	int m_score;
	int m_pHealth;
	int m_ammo;
};

