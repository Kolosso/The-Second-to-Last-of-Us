// COMP710 GP 2D Framework

// This includes:
#include "inputhandler.h"

// Local includes:
#include "game.h"

// Library includes:
#include <cassert>
#include <iostream>

InputHandler::InputHandler()
: mp_GameController(0)
, mi_deadZone(8000)
{
}

InputHandler::~InputHandler()
{
	if (mp_GameController)
	{
		SDL_GameControllerClose(mp_GameController);
		mp_GameController = 0;
	}
}

bool
InputHandler::Initialise()
{
	//SDL_Init(SDL_INIT_GAMECONTROLLER);
	int numControllers = SDL_NumJoysticks();
	
	//debug stuff
	for (int i = 0; i < SDL_NumJoysticks(); ++i) {
		const char *name = SDL_GameControllerNameForIndex(i);
		if (name) {
			printf("Joystick %i has game controller name '%s'\n", i, name);
		}
		else {
			printf("Joystick %i has no game controller name.\n", i);
		}
	}

	//Something like this may be used to call multiple controllers.
	//currently used to get the first available controller
	for (int i = 0; i < SDL_NumJoysticks(); ++i) {
		if (SDL_IsGameController(i)) {
			mp_GameController = SDL_GameControllerOpen(i);
			std::cout << SDL_GetError() << std::endl;
			if (mp_GameController) {
				std::cout << "Controller " << i << " connected" << std::endl;

				break;
			}
		}
	}

	//if (numControllers > 0)
	//{
	//	if (SDL_IsGameController(0)) {
	//		mp_GameController = SDL_GameControllerOpen(0);
	//		std::cout << SDL_GetError() << std::endl;
	//		if (mp_GameController) {
	//			std::cout << "Controller " << 0 << " connected" << std::endl;

	//		}
	//	}
	//}
	//else
	//{
	//	std::cout << "No Controller Connected." << std::endl;
	//}
	//assert(mp_GameController); // COMMENTED OUT FOR TESTING
	return(true);
}

void
InputHandler::ProcessInput(Game &game)
{
	// Receive Input Events Below...
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			game.Quit();
		}
		else if (e.type == SDL_KEYDOWN)
		{
			// Audio volume UP
			if (e.key.keysym.sym == SDLK_EQUALS) {
				game.VolumeAdjust(true);
			}
			// Audio volume DOWN
			if (e.key.keysym.sym == SDLK_MINUS) {
				game.VolumeAdjust(false);
			}
		}
		else if (e.type == SDL_CONTROLLERBUTTONDOWN)
		{
			if (*game.GetGameState() == PLAYING)
			{
				if (e.jbutton.button == 0)
				{
					// Perform action
					game.ShootGun();
				}
				else if (e.cbutton.button == SDL_CONTROLLER_BUTTON_START)
				{
					// Perform action
					game.PauseGame();
				}
				else if (e.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
				{
					game.ShootGun();
				}
			}
			else
			{
				if (e.cbutton.button == 0)
				{
					// Perform action
					game.GetMenuManager()->MenuEnter(game.GetGameState());
				}
				else if (e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
				{
					// Perform action
					game.GetMenuManager()->MenuDown(game.GetGameState());
				}
				else if (e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
				{
					// Perform action
					game.GetMenuManager()->MenuUp(game.GetGameState());
				}
			}
		}
		else if (e.type == SDL_CONTROLLERAXISMOTION)
		{
			
			if (e.caxis.axis < 2 && (e.caxis.value < -mi_deadZone || e.caxis.value > mi_deadZone))
			{
				game.GetPlayer()->Move(SDL_GameControllerGetAxis(mp_GameController, SDL_CONTROLLER_AXIS_LEFTX), SDL_GameControllerGetAxis(mp_GameController, SDL_CONTROLLER_AXIS_LEFTY));
			}

			if ((SDL_GameControllerGetAxis(mp_GameController, SDL_CONTROLLER_AXIS_LEFTX) < mi_deadZone && SDL_GameControllerGetAxis(mp_GameController, SDL_CONTROLLER_AXIS_LEFTX) > -mi_deadZone)
				&& (SDL_GameControllerGetAxis(mp_GameController, SDL_CONTROLLER_AXIS_LEFTY) < mi_deadZone && SDL_GameControllerGetAxis(mp_GameController, SDL_CONTROLLER_AXIS_LEFTY) > -mi_deadZone))
			{
				game.GetPlayer()->StopMove();
			}

			if (e.caxis.axis > 1 
				&& ((SDL_GameControllerGetAxis(mp_GameController, SDL_CONTROLLER_AXIS_RIGHTX) > mi_deadZone || SDL_GameControllerGetAxis(mp_GameController, SDL_CONTROLLER_AXIS_RIGHTX) < -mi_deadZone)
				|| (SDL_GameControllerGetAxis(mp_GameController, SDL_CONTROLLER_AXIS_RIGHTY) > mi_deadZone || SDL_GameControllerGetAxis(mp_GameController, SDL_CONTROLLER_AXIS_RIGHTY) < -mi_deadZone)))
			{
				game.GetPlayer()->Rotate(SDL_GameControllerGetAxis(mp_GameController, SDL_CONTROLLER_AXIS_RIGHTX), SDL_GameControllerGetAxis(mp_GameController, SDL_CONTROLLER_AXIS_RIGHTY));
			}
		}
	}
}