#include "EnemyManager.h"
#include "EnemyEntity.h"

// Default Constructor - Not Used
CEnemyManager::CEnemyManager(void)
{
}

// Main Constructor - Used to set the private values for the area an enemy
// can spawn and the size of the enemy images
// Also sets the default values for the other member variables
CEnemyManager::CEnemyManager(int destWidth, int destHeight, int frameX, int frameY) 
	: m_destWidth(destWidth), m_destHeight(destHeight), m_frameX(frameX), m_frameY(frameY)
{
	m_ticksBetweenEnemySpawns = 30;
	m_ticksLastSpawn = 0;
	m_newEnemy = false;
	m_minimumTimeBetweenSpawns = 30;
}

// Default Destructor
CEnemyManager::~CEnemyManager(void)
{
}

// Update Method - Checks if a new enemy should be spawned every tick
void CEnemyManager::Update(int ticks)
{
	// If the tick value is greater than the ticks since the last spawn
	// added onto the ticks between spawns, a new enemy will be flagged to be spawned
	if (ticks > m_ticksLastSpawn + m_ticksBetweenEnemySpawns)
	{
		// Set the ticksSinceLastSpawn to be the value of ticks sent in this update
		m_ticksLastSpawn = ticks;

		// randomise the value between now and the next spawn, with a minimum value
		// of the minimumTimeBetweenSpawns
		m_ticksBetweenEnemySpawns = (rand() % 70) + m_minimumTimeBetweenSpawns;

		// Flag a new enemy to be created
		m_newEnemy = true;

		// Decrement the minimum time between spawns
		m_minimumTimeBetweenSpawns -= 5;
	}

	// Else, make sure flag is set to false, or enemies will constantly
	// be created every tick
	else
	{
		m_newEnemy = false;
	}
}

// Method to create a new enemy to be used by the Entity List in the
// WorldModel. The position is 'randomised' to be between the top left and top right
// To extend this, I could create different types of enemies based on a switch statement
CEnemyEntity* CEnemyManager::NewEnemy(int graphicsID, int id)
{
	int x = rand() % (m_destWidth - 60) + 30;
	CEnemyEntity* newEnemy = new CEnemyEntity(graphicsID, x, -m_frameY, m_frameX, m_frameY, 5, 2, 3, 600);
	
	return newEnemy;
}

// Getter - Shows current flag status to WorldModel
const bool CEnemyManager::NeedNewEnemy()
{
	return m_newEnemy;
}