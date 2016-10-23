// COMP710 GP 2D Framework

// Local includes:
#include "Game.h"
#include "LogManager.h"

// Library includes:
#include <SDL.h>
//Visual Leak Detector:
#include <vld.h>

int main(int argc, char* argv[])
{
	Game& gameInstance = Game::GetInstance();
	if (!gameInstance.Initialise())
	{
		LogManager::GetInstance().Log("Game Init Failed!");
		return (1);
	}

	while (gameInstance.DoGameLoop())
	{
		// No body
	}

	Game::DestoryInstance();

	return (0);
}