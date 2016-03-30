#include "PlayerEntity.h"
#include "Visualisation.h"

// Main Constructor - Uses parent's main constructor to assign the
// values for the Graphics ID, starting position, image frame size,
// health and damage properties, whilst also setting the player's
// maximum amount of bullets.
// Also assigns the values of the current number of bullets, speed
// and side.
CPlayerEntity::CPlayerEntity(int graphicsID, int posX, int posY, int frameX, int frameY, int health, int damage, int maxBullets)
	: CEntity(graphicsID, posX, posY, frameX, frameY, health, damage)
{
	m_maxBullets = maxBullets;
	m_bullets = maxBullets;
	m_speedX = 30; m_speedY = 30;
	m_side = Side::player;
	
}

// Default Destructor
CPlayerEntity::~CPlayerEntity(void)
{
}

// Update Method - In the Player's Method, we use the Keyboard and Controller Data
// to handle Input, determining the player movement and actions on that.
void CPlayerEntity::Update(HAPI_TKeyboardData kData, HAPI_TControllerData cData)
{
	// Set the previous positions to be the current positions
	m_prevPosX = m_posX;
	m_prevPosY = m_posY;
	m_clampS = false;

	// Check to see if the clip is empty
	if(m_bullets == 0)
		m_emptyClip = true;
	else
		m_emptyClip = false;

	// Flag determining if the keyboard has been used
	bool usedKeyboard = false;

	// Keyboard Input (Arrow Keys to Control Object)
	if(HAPI->GetKeyboardData(&kData))
	{
		// Base values for total X and Y moved this turn.
		int totalX = 0, totalY = 0;
		// Left Arrow - Move Left
		if(kData.scanCode[HK_LEFT])
		{
			totalX -= m_speedX;
			usedKeyboard = true;
		}
		// Up Arrow - Move Up
		if(kData.scanCode[HK_UP])
		{
			totalY -= m_speedY;
			usedKeyboard = true;
		}
		// Right Arrow - Move Right
		if(kData.scanCode[HK_RIGHT])
		{
			totalX += m_speedX;
			usedKeyboard = true;
		}
		// Down Arrow - Move Down
		if(kData.scanCode[HK_DOWN])
		{
			totalY += m_speedY;
			usedKeyboard = true;
		}

		// Total the Positions
		m_posX += totalX;
		m_posY += totalY;
		m_collisionRect.Translate(totalX, totalY);

		// Space Key
		if(kData.scanCode[HK_SPACE] && m_bullets > 0)
		{
 			m_hasShot = true;
			m_bullets--;
			usedKeyboard = true;
		}
		else
		{
			m_hasShot = false;
		}

		if(kData.scanCode['R'] && !kData.scanCode[HK_SPACE] && m_bullets == 0)
		{
			m_bullets = m_maxBullets;
			usedKeyboard = true;
		}
	}

	// Controller Input (D-Pad/Thumbstick for 8 directions)
	if(HAPI->GetControllerData(0, &cData) && !usedKeyboard)
	{
		// Normalize values between -1.0f and 1.0f
		int lThumbX = (cData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X]);
		int lThumbY = (cData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y]);
		
		int totalX = 0, totalY = 0;

		// Left on D-Pad/Thumbstick
		if(cData.digitalButtons[HK_DIGITAL_DPAD_LEFT] ||
			lThumbX < -HK_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			//m_prevPosX = m_posX;
			totalX -= m_speedX;
			m_clampS = false;
		}
		// Up on D-Pad/Thumbstick
		if(cData.digitalButtons[HK_DIGITAL_DPAD_UP] ||
			lThumbY > HK_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			//m_prevPosY = m_posY;
			totalY -= m_speedY;
			m_clampS = false;
		}
		// Right on D-Pad/Thumbstick
		if(cData.digitalButtons[HK_DIGITAL_DPAD_RIGHT] ||
			lThumbX > HK_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			//m_prevPosX = m_posX;
			totalX += m_speedX;
			m_clampS = false;
		}
		// Down on D-Pad/Thumbstick
		if(cData.digitalButtons[HK_DIGITAL_DPAD_DOWN] ||
			lThumbY < -HK_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			//m_prevPosY = m_posY;
			totalY += m_speedY;
			m_clampS = false;
		}

		m_posX += totalX;
		m_posY += totalY;
		m_collisionRect.Translate(totalX, totalY);

		if(cData.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER] > HK_GAMEPAD_TRIGGER_THRESHOLD &&
			m_bullets > 0)
		{
			m_hasShot = true;
			m_bullets--;
		}
		if(cData.digitalButtons[HK_DIGITAL_X] && m_bullets == 0 &&
			cData.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER] <= HK_GAMEPAD_TRIGGER_THRESHOLD)
		{
			m_bullets = m_maxBullets;
		}
	}

	// Clamp the position and Rectangle within the bounds
	if (m_posX < 0)
	{
		m_collisionRect.Translate(-m_posX, 0);
		m_posX = 0;
	}
	else if (m_posX > 480)
	{
		m_collisionRect.Translate(480 - m_posX, 0);
		m_posX = 480;
	}

	if (m_posY < 100)
	{
		m_collisionRect.Translate(0, 100 - m_posY);
		m_posY = 100;
	}
	else if (m_posY > 568)
	{
		m_collisionRect.Translate(0, 568 - m_posY);
		m_posY = 568;
	}

}

// Rendering Method - All Drawing to screen occurs here,
// using the passed through Visualisation and lerp value 's'
void CPlayerEntity::Render(CVisualisation *vis, float s)
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

// Destroy Method - Set to Inactive and Delete
void CPlayerEntity::Destroy()
{
	m_active = false;
}