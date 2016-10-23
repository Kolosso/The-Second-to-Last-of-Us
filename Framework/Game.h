#ifndef __GAME_H__
#define __GAME_H__
#include "BackBuffer.h"
#include "InputHandler.h"
#include "Zombie.h"
#include "Player.h"
#include "Car.h"
#include "Gasoline.h"
#include "Distraction.h"
#include "MenuManager.h"
#include "GameStates.h"

// Library includes:
#include <Box2D.h>

// Forward declarations
class LevelManager;
class AudioManager;

class Game
{
	// Member Methods:
public:
	static Game& GetInstance();
	static void DestoryInstance();
	~Game();

	bool Initialise();
	bool DoGameLoop();
	void Quit();

	void VolumeAdjust(bool v);

	Player* GetPlayer();

	void ShootGun();

	AudioManager* GetAudioManager();
	MenuManager* GetMenuManager();
	GameState* GetGameState();

	void PauseGame();
	void RestartGame();

protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void LevelBuild();
	void LevelMove();
	void LevelMoveIndividual(float x, float y);
	void LevelMoveContainerAssets(float x, float y);
	
	void SpawnZombie(int x, int y);

private:
	Game(const Game& game);
	Game& operator=(const Game& game);

	Game();

	// Member Data:
public:
		
protected:
	static Game* sp_Instance;
	BackBuffer* mp_backBuffer;
	InputHandler* mp_inputHandler;
	LevelManager* mp_levelManager;
	AudioManager* mp_audioManager;
	MenuManager* mp_menuManager;

	GameState* mp_gameState;

	bool mb_looping;

	float mf_executionTime;
	float mf_elapsedSeconds;
	float mf_lag;
	int mi_frameCount;
	int mi_FPS;
	int mi_numUpdates;
	int mi_lastTime;
	bool mb_drawDebugInfo;

	int mi_totalBullets;
	int mi_currentBullet;

	Player* mp_player;
	Car* mp_car;
	Gasoline* mp_gasoline;
	Distraction* mp_distraction;
	std::vector<Zombie*> mp_zombies;
	std::vector<Bullet*> mp_bullets;


	//Box2D
	b2World* gameWorld;
private:

};

#endif //__GAME_H__