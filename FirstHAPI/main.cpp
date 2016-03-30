// Include Libraries
#include <HAPI_lib.h>
#include "WorldModel.h"

// HAPI's Main Function
void HAPI_Main()
{
	// Variable Declaration
	CWorldModel *worldModel = new CWorldModel();

	// Initialise the world by loading the level
	worldModel->LoadLevel(512, 728);
	
	// Update the program
	while(HAPI->Update())
	{
		// All game code will be executed inside this loop
		worldModel->Update();
	}

	// Delete the World Model pointer
	delete worldModel;
}