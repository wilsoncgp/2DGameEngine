#include "SoundManager.h"

// Default Constructor - Initialises the Sound class from FMODSound
CSoundManager::CSoundManager(void)
{
	Sound::Init();
}

// Default Destructor - deletes all sounds and clears the vector list
CSoundManager::~CSoundManager(void)
{
	for(SoundEffect* sound : m_sounds)
	{
		delete sound;
	}
	m_sounds.clear();
}

// Loads a Sound into FMODSound and passes back an ID
bool CSoundManager::LoadSound(char* filename, int* id)
{
	// Create a new sound from the file-name and create the ID
	SoundEffect* newSound = new SoundEffect(filename);
	*id = m_sounds.size();

	// before adding the sound to the list
	m_sounds.push_back(newSound);

	return true;
}

// Plays the Sound using FMODSound's SoundEffect Play Method
void CSoundManager::PlaySound(int id)
{
	m_sounds[id]->Play();
}