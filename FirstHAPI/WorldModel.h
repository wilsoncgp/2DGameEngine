#pragma once
#include<HAPI_lib.h>
#include <vector>

// Declare already made classes, to avoid tangled headers
class CVisualisation;
class CEntity;
class CEnemyManager;
class CSoundManager;

/*
*
*	A black boxed component which
*	holds a list of entities and
*	calls for each of them to be
*	updated and drawn to the screen.
*
*	It updates all the entities, 
*	whilst also handling all the
*	flags from the entities and
*	managers. Also handles things
*	such as collisions and deleting
*	entities out of bounds or destroyed.
*
*	The world is updated every 'tick',
*	the drawing is lerped every loop.
*	This makes movement smoother and
*	a lot easier to control and making
*	the engine have less computations
*	to do per loop.
*
*/

class CWorldModel
{
public:
	// Constructor and Destructor

	CWorldModel(void);
	~CWorldModel(void);

	// Methods to Load and Update the World

	bool LoadLevel(int sW, int sH);
	void Update();

private:
	// Private method to draw
	void Draw(float s);
	
	// Private variables -
	// entities - list of all the entities in the game
	// vis - the Visualisation component
	// enemyManager - the Enemy Manager
	// soundManager - the Sound Manager
	// screenWidth/Height - the screen width and height
	// player/enemy/background/bullet/hud/health/explosionIDs - graphics IDs to each sprite
	// explosion/laserSoundIDs - sound IDs to each playable sound effect
	// keyboardData - the data about keys pressed on the keyboard
	// controllerData - the data about the analogue and digital buttons on the Xbox 360 controller
	// screenColour - the colour of the cleared screen
	// score - the world model's record of the current score
	// enemiesKilled - keeps track of the number of enemies killed for end state
	// playerKilled - flag to see if player is dead
	// timeBetweenUpdates - the time between each update/tick
	// time - the current time in the game
	// ticks - current amount of ticks gone through in the game
	// bulletWidth/Height - the size of the bullet sprite
	// explosionWidth/Height - the size of the explosion sprites
	std::vector<CEntity*> m_entities;
	CVisualisation * m_vis;
	CEnemyManager *m_enemyManager;
	CSoundManager *m_soundManager;

	int m_screenWidth, m_screenHeight;
	int playerID, enemyID, backgroundID, bulletID, hudID, healthID, explosionID;
	int explosionSoundID, laserSoundID;
	HAPI_TKeyboardData m_keyboardData;
	HAPI_TControllerData m_controllerData;
	HAPI_TColour m_screenColour;
	int m_score;
	int m_enemiesKilled;
	bool m_playerKilled;

	DWORD m_timeBetweenUpdates;
	DWORD m_time;
	int m_ticks;

	int m_bulletWidth, m_bulletHeight;
	int m_explosionWidth, m_explosionHeight;
};

