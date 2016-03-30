#pragma once
#include <HAPI_lib.h>
#include <vector>
#include "FMODSound.h"

/*
*
*	A Sound Manager class to hold sounds.
*	The manager will pass out an ID to each
*	loaded sound.
*	Sound is loaded and played using example
*	code with FMOD.
*
*	Example code 'FMODSound' classes are provided
*	by Anthony Ulrich, of 'thatnoobprogrammer.blogspot.co.uk'.
*	I do not claim any of the code in his classes
*	to be my own.
*
*/
class CSoundManager
{
public:
	// Constructor and Destructor
	CSoundManager(void);
	~CSoundManager(void);

	// Methods to Load and Play Sounds
	bool LoadSound(char* filename, int* id);
	void PlaySound(int id);

private:
	// Private variable -
	// sounds - a vector list of SoundEffects
	std::vector<SoundEffect*> m_sounds;
};

