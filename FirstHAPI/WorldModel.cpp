#include "WorldModel.h"
#include "Visualisation.h"
#include "PlayerEntity.h"
#include "EnemyEntity.h"
#include "BulletEntity.h"
#include "BackgroundEntity.h"
#include "HUDEntity.h"
#include "ExplosionEntity.h"
#include "EnemyManager.h"
#include "SoundManager.h"

// Default Constructor - Declare new Visualisation and Sound Manager objects
CWorldModel::CWorldModel(void)
{
	m_vis = new CVisualisation() ;
	m_soundManager = new CSoundManager();
}

// Default Destructor - Delete the Visualisation, Enemy/Sound Manager objects
// as well as all the entities in the list and clearing the vector list
CWorldModel::~CWorldModel(void)
{
	delete m_vis;
	delete m_enemyManager;
	delete m_soundManager;
	for(CEntity* e : m_entities)
	{
		delete e;
	}
	m_entities.clear();
}

// The main Initialisation for the entire game - Initialises all private members and
// loads all textures/sounds, while adding the first entities to the list
bool CWorldModel::LoadLevel(int sW, int sH)
{
	// Set the screenWidth and Height, but may be changed when Initialising the
	// Visualisation component
	m_screenWidth = sW; m_screenHeight = sH;
	if(!m_vis->Initialise(m_screenWidth, m_screenHeight))
		return false;

	// Declare integers for each load to take out the width and height of each texture
	// Then load each texture using the correct file-names from the project folder 
	// and passing through an ID to them. If any of the textures fail to load, an 
	// error message will pop up and the game will be forced to exit.

	// Player Texture
	int objectPosX, objectPosY;
	int objFrameW, objFrameH;
	//objFrameW = 64; objFrameH = 128;
	//if(!m_vis->LoadTexture("peanutbutterjelly.png", &playerID, objFrameW, objFrameH, true, true, 100))
	//	return false;
	if(!m_vis->LoadTexture("playership.png", &playerID, objFrameW, objFrameH))
		return false;

	// Background texture
	int bgWidth, bgHeight;
	/*if(!m_vis->LoadTexture("background.tga", &backgroundID, bgWidth, bgHeight, true, true, 100))
		return false;*/
	if (!m_vis->LoadTexture("scrollingbackground.png", &backgroundID, bgWidth, bgHeight))
		return false;

	// Enemy Texture
	int enemyWidth, enemyHeight;
	if(!m_vis->LoadTexture("enemyship.png", &enemyID, enemyWidth, enemyHeight))
		return false;

	// Bullet Texture
	if(!m_vis->LoadTexture("bullet.png", &bulletID, m_bulletWidth, m_bulletHeight))
		return false;

	// HUD Texture
	int hudWidth, hudHeight;
	if(!m_vis->LoadTexture("HUD.png", &hudID, hudWidth, hudHeight))
		return false;

	// Health Bar Texture
	int healthWidth, healthHeight;
	if (!m_vis->LoadTexture("healthbar.png", &healthID, healthWidth, healthHeight))
		return false;

	// Explosion Texture
	m_explosionWidth = enemyWidth; m_explosionHeight = enemyHeight;
	if (!m_vis->LoadTexture("explosions.png", &explosionID, m_explosionWidth, m_explosionHeight, true, true, 80))
		return false;

	// Load sounds using the correct file-names or assertions will fail and the program will break
	if (!m_soundManager->LoadSound("explosionsound1.wav", &explosionSoundID))
		return false;
	if (!m_soundManager->LoadSound("laser1.wav", &laserSoundID))
		return false;
	
	// Set member variables
	m_screenColour = HAPI_TColour(130, 30, 30);
	m_timeBetweenUpdates = 80;
	m_time = HAPI->GetTime();
	m_ticks = 0;
	m_score = 0;
	m_playerKilled = false;
	m_enemiesKilled = 0;
	
	// Set initial player position
	objectPosX = 268; objectPosY = 500;

	// Add the new entities
	// Background Entity
	CBackgroundEntity *background = new CBackgroundEntity(backgroundID, 0, 0, bgWidth, bgHeight, 1, 1);
	m_entities.push_back(background);

	// Player Entity
	CPlayerEntity *player = new CPlayerEntity(playerID, objectPosX, objectPosY, objFrameW, objFrameH, 100, 1, 10);
	m_entities.push_back(player);
	
	// HUD Entity
	CHUDEntity *hud = new CHUDEntity(hudID, 0, 600, hudWidth, hudHeight, 1, 1, healthID, bulletID);
	m_entities.push_back(hud);

	// Make a new Enemy Manager based on above information
	m_enemyManager = new CEnemyManager(m_screenWidth, m_screenHeight, enemyWidth, enemyHeight);

	return true;
}

// The main update loop of the World Model
void CWorldModel::Update()
{
	// Set the new time right now
	DWORD timeNow = HAPI->GetTime();
	// If time has been reached, the next tick can start
	if(timeNow - m_time > m_timeBetweenUpdates)
	{
		
		// Set the current size of the entities list
		unsigned int size = m_entities.size();

		// Update the Enemy Manager to find out if a new Enemy is needed
		m_enemyManager->Update(m_ticks);
		// If so, take a new one from the enemy manager and increase the size of the entities list variable
		if(m_enemyManager->NeedNewEnemy())
		{
			m_entities.push_back(m_enemyManager->NewEnemy(enemyID, size));
			size++;
		}

		// Update all Entities in vector list
		for(unsigned int i = 0; i < size; i++)
		{
			m_entities[i]->Update(m_keyboardData, m_controllerData);

			

			// Check if Entities have shot and their clip isn't empty
			if (m_entities[i]->HasShot() && !m_entities[i]->GetEmptyClip())
			{
				// If the entity that shot was a player, shoot up and play the laser sound
				if(m_entities[i]->GetSide() == Side::player)
				{
					m_entities.push_back(new CBulletEntity(bulletID,
															m_entities[i]->GetPosX() + (m_entities[i]->GetCollisionRectangle().Width() / 2),
															m_entities[i]->GetPosY() - m_bulletHeight - 10,
															m_bulletWidth,
															m_bulletHeight,
															1,
															5,
															Side::player));
					m_soundManager->PlaySound(laserSoundID);
				}
				// Else, if it was an enemy's shot, shoot downwards and play the laser sound
				else if (m_entities[i]->GetSide() == Side::enemy)
				{
					m_entities.push_back(new CBulletEntity(bulletID,
															m_entities[i]->GetPosX() + (m_entities[i]->GetCollisionRectangle().Width() / 2),
															m_entities[i]->GetPosY() + m_entities[i]->GetCollisionRectangle().Height() + 10,
															m_bulletWidth,
															m_bulletHeight,
															1,
															5,
															Side::enemy));
					m_soundManager->PlaySound(laserSoundID);
				}
			}

			// Check if the bullets are either out of bounds or inactive and delete them if so
			// using an iterator
			if (dynamic_cast<CBulletEntity*>(m_entities[i]) && 
				(m_entities[i]->GetPosY() > m_screenHeight - 150 || m_entities[i]->GetPosY() < -m_bulletHeight
				|| !m_entities[i]->IsActive()))
			{
				bool exitCond = false;
				std::vector<CEntity*>::iterator it = m_entities.begin();
				while(!exitCond)
				{
					if (*it == m_entities[i])
					{
						delete m_entities[i];
						m_entities.erase(it);
						exitCond = true;
						i--;
						size--;
					}
					else
					{
						it++;
					}

				}
			}
		}

		// Collision Checking
		for(unsigned int i = 0; i < size; i++)
		{
			// If the entity isn't a neutral object
			if(m_entities[i]->GetSide() != Side::neutral)
			{
				// Loop through the rest of the objects specifically only check one against the other once
				for(unsigned int j = i + 1; j < size; j++)
				{
					// If the next indexed entity isn't a neutral entity,
					// if the side's of both entities are not equal,
					// and if the collision rectangles of both are colliding
					if(m_entities[j]->GetSide() != Side::neutral &&
						m_entities[i]->GetSide() != m_entities[j]->GetSide() &&
						m_entities[i]->GetCollisionRectangle().Collide(m_entities[j]->GetCollisionRectangle()))
					{
						// Damage both entities with the other's damage value
						m_entities[i]->Damage(m_entities[j]->GetDamage());
						m_entities[j]->Damage(m_entities[i]->GetDamage());

						// If the player has destroyed an enemy, increment the score
						if((m_entities[i]->GetSide() == Side::player && !m_entities[j]->IsActive() && dynamic_cast<CEnemyEntity*>(m_entities[j])) ||
							(m_entities[j]->GetSide() == Side::player && !m_entities[i]->IsActive() && dynamic_cast<CEnemyEntity*>(m_entities[i])))
						{
							m_score+=100;
							m_enemiesKilled++;
							// Check which of the entities was an enemy and draw an explosion at that point
							if(m_entities[i]->GetSide() == Side::enemy)
								m_entities.push_back(new CExplosionEntity(explosionID, m_entities[i]->GetPosX(), m_entities[i]->GetPosY(),
																m_explosionWidth, m_explosionHeight, 1, 1, (rand() % 8), 15));
							else if(m_entities[j]->GetSide() == Side::enemy)
							{
								m_entities.push_back(new CExplosionEntity(explosionID, m_entities[j]->GetPosX(), m_entities[j]->GetPosY(),
																m_explosionWidth, m_explosionHeight, 1, 1, (rand() % 8), 15));
							}

							// Play the explosion sound
							m_soundManager->PlaySound(explosionSoundID);
						}
					}
				}
			}
		}

		// Set HUD elements
		for(unsigned int i = 0; i < size; i++)
		{
			// Check for the first entity to be a player
			if (dynamic_cast<CPlayerEntity*>(m_entities[i]))
			{
				// When the player is found, find the HUD Entity
				for(unsigned int j = 0; j < size; j++)
				{
					// When it is found...
					if(dynamic_cast<CHUDEntity*>(m_entities[j]))
					{
						// ... set the values to be that of the player's
						dynamic_cast<CHUDEntity*>(m_entities[j])->SetPlayerHealth(m_entities[i]->GetHealth());
						dynamic_cast<CHUDEntity*>(m_entities[j])->SetAmmo(m_entities[i]->GetBullets());
						dynamic_cast<CHUDEntity*>(m_entities[j])->SetScore(m_score);

						// Exit the loops now the attributes have been set
						j = size;
					}
				}

				// Again, exit the loop, the deed is done
				i = size;
			}
		}

		
		if(m_playerKilled || m_enemiesKilled > 20)
		{
			for (CEntity* e : m_entities)
			{
				if(e->GetSide() != Side::neutral)
				{
					e->Destroy();
				}
			}
		}

		// Delete all Inactive Entities
		// Use an iterator to run through and delete all inactive, non-neutral entities
		// from the list. Alter the list size whilst going through to make sure no entity
		// is left floating through space when it's meant to be dead!
		for(unsigned int i = 0; i < size; i++)
		{
			if(!m_entities[i]->IsActive() && m_entities[i]->GetSide() != Side::neutral)
			{
				if (dynamic_cast<CPlayerEntity*>(m_entities[i]))
				{
					m_playerKilled = true;
				}
				bool exitCond = false;
				std::vector<CEntity*>::iterator it = m_entities.begin();
				while(!exitCond)
				{
					if (*it == m_entities[i])
					{
						delete m_entities[i];
						m_entities.erase(it);
						exitCond = true;
						i--;
						size--;
					}
					else
					{
						it++;
					}

				}
			}
		}


		// Reset Timer
		m_time = timeNow;
		m_ticks++;
	}
	
	// Render scene using the s value, the lerp value
	float s = (timeNow - m_time) / (float) m_timeBetweenUpdates;
	Draw(s);

	// Show the FPS rate on screen
	HAPI->SetShowFPS(true, 50, 50);
}

// Private method used to draw all entities, based on the lerp value
void CWorldModel::Draw(float s)
{
	// Using the ClearScreen method in Visualisation, clear the screen to a colour
	m_vis->ClearScreen(m_screenColour);

	// Put entity size into a variable to optimise looping
	int entity_size = m_entities.size();
	
	// Loop through entities and render each one
	for(int i = 0; i < entity_size; i++)
		m_entities[i]->Render(m_vis, s);

	if (m_playerKilled || m_enemiesKilled > 20)
	{
		if (m_enemiesKilled > 20)
		{
			m_vis->RenderText(200, 300, HAPI_TColour(130, 30, 30), "CONGRATULATIONS!");
			m_vis->RenderText(200, 330, HAPI_TColour(130, 30, 30), "Your score was: " + std::to_string(m_score));
		}
		else
		{
			m_vis->RenderText(200, 300, HAPI_TColour(130, 30, 30), "GAME OVER!");
		}
	}
}