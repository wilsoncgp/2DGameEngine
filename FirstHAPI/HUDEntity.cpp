#include "HUDEntity.h"
#include "Visualisation.h"

// Default Constructor - Not Used
CHUDEntity::CHUDEntity(void)
{
}

// Main constructor - Uses parent's main constructor to assign the
// values for the Graphics ID, starting position, image frame size,
// health and damage properties, whilst also setting the HUD's
// IDs for the Health and Bullet textures
// Also sets the Side, the speed, and the initial variables
// for the player health, ammo and score
CHUDEntity::CHUDEntity(int graphicsID, int posX, int posY, int frameX, int frameY, int health, int damage, int healthID, int bulletID)
	:CEntity(graphicsID, posX, posY, frameX, frameY, health, damage), m_healthID(healthID), m_bulletID(bulletID)
{
	m_side = Side::neutral;
	m_speedX = 0; m_speedY = 0;
	m_pHealth = 100;
	m_ammo = 10;
	m_score = 0;
}

// Default Constructor
CHUDEntity::~CHUDEntity(void)
{
}

// Update Method - Not used right now
void CHUDEntity::Update(HAPI_TKeyboardData kData, HAPI_TControllerData cData)
{

}

// Render Method - Draws the main texture for the HUD,
// then uses the other 2 textures and text to draw on
// the values for the health, ammo and score, using the
// passed through Visualisation
void CHUDEntity::Render(CVisualisation *vis, float s)
{
	// Draw the initial HUD texture
	vis->DrawTexture(m_graphicsID, m_posX, m_posY, false);

	// If the health is greater than 0,
	// draw the health bar using the specialised Visualisation method
	if(m_pHealth > 0)
		vis->DrawHealthBar(m_healthID, m_posX + 24, m_posY + 28, m_pHealth);

	// If the ammo is greater than 0, draw a Bullet texture for each bullet
	if(m_ammo > 0)
	{
		for(int i = 0; i < m_ammo; i++)
		{
			vis->DrawTexture(m_bulletID, m_posX + 30 + i*12, m_posY + 76);
			// if (double ammo)
			//vis->DrawTexture(m_bulletID, m_posX + 30 + i*12, m_posY + 92);
		}
	}

	// If the score is a non-negative value, draw it into the score box
	if(m_score >= 0)
	{
		vis->RenderText(m_posX + 330, m_posY + 25, HAPI_TColour(), std::to_string(m_score));
	}
}

// Destroy Method - Not really used
void CHUDEntity::Destroy()
{
	m_health = 0;
}

// Setter - Set the Score Value
void CHUDEntity::SetScore(int score)
{
	m_score = score;
}

// Setter - Set the Player Health Value
void CHUDEntity::SetPlayerHealth(int health)
{
	m_pHealth = health;
}

// Setter - Set the Ammo Value
void CHUDEntity::SetAmmo(int ammo)
{
	m_ammo = ammo;
}