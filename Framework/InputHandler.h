// COMP710 GP 2D Framework
#ifndef __INPUTHANDLER_H__
#define __INPUTHANDLER_H__

// Library includes:
#include <SDL.h>

// Forward declarations:
class Game;

class InputHandler
{

	// Member methods:
public:
	InputHandler();
	~InputHandler();

	bool Initialise();
	void ProcessInput(Game &game);

protected:

private:
	InputHandler(const InputHandler& inputHandler);
	InputHandler& operator=(const InputHandler& inputHandler);

	// Member data;
public:

protected:
	SDL_GameController* mp_GameController;
	int mi_deadZone;

private:

};

#endif // __INPUTHANDLER_H__