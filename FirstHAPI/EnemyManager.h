#pragma once
#include <vector>

// Declare already made classes, to avoid tangled headers
class CEnemyEntity;

/*
*
*	The EnemyManager class is used to determine
*	when a new Enemy needs to be created, and handles creation,
*	and	communicates with the WorldModel via a
*	boolean Getter.
*
*/

class CEnemyManager
{
public:
	// Constructors and Destructor

	CEnemyManager(void);
	CEnemyManager(int destWidth, int destHeight, int frameX, int frameY);
	~CEnemyManager(void);

	// Update method
	void Update(int ticks);

	// Method to create a new enemy
	CEnemyEntity* NewEnemy(int graphicsID, int id);

	// Getter to tell WorldModel a new enemy is needed
	const bool NeedNewEnemy();

private:
	// Private variables -
	// newEnemy - used to communicate with the WorldModel when a new enemy is needed
	// ticksBetweenEnemySpawns - used for working out time between spawns
	// ticksLastSpawn - ticks value at last spawn
	// minimumTimeBetweenSpawns - the absolute minimum ticks between a new spawn
	// destWidth, destHeight - area enemies can spawn
	// frameX, frameY - size of enemy
	bool m_newEnemy;
	int m_ticksBetweenEnemySpawns;
	int m_ticksLastSpawn;
	int m_minimumTimeBetweenSpawns;
	int m_destWidth, m_destHeight;
	int m_frameX, m_frameY;
};

