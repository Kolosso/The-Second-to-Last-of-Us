// COMP710 GP 2D Framework

// This includes:
#include "Game.h"

// Local includes:
#include "BackBuffer.h"
#include "LogManager.h"
#include "InputHandler.h"
#include "LevelManager.h"
#include "AudioManager.h"
#include "Sprite.h"
#include "Entity.h"
#include "Character.h"
#include "ZombieStates.h"

// Library includes:
#include <cassert>
#include <SDL.h>

// Static Members:
Game* Game::sp_Instance = 0;

Game&
Game::GetInstance()
{
	if (sp_Instance == 0)
	{
		sp_Instance = new Game();
	}

	assert(sp_Instance);

	return (*sp_Instance);
}

void
Game::DestoryInstance()
{
	delete sp_Instance;
	sp_Instance = 0;
}

Game::Game()
: mp_backBuffer(0)
, mp_inputHandler(0)
, mp_levelManager(0)
, mp_audioManager(0)
, mp_menuManager(0)
, mp_gameState(0)
, mb_looping(true)
, mf_executionTime(0)
, mf_elapsedSeconds(0)
, mf_lag(0)
, mi_frameCount(0)
, mi_FPS(0)
, mi_numUpdates(0)
, mi_lastTime(0)
, mb_drawDebugInfo(false)
, mi_totalBullets(6)
, mi_currentBullet(0)
//Entities
, mp_player(0)
, mp_car(0)
, mp_gasoline(0)
, mp_distraction(nullptr)
, gameWorld(nullptr)
{
}


Game::~Game()
{
	// Utilities
	delete mp_inputHandler;
	mp_inputHandler = 0;

	delete mp_backBuffer;
	mp_backBuffer = 0;

	delete mp_levelManager;
	mp_levelManager = 0;

	delete mp_audioManager;
	mp_audioManager = 0;

	delete mp_menuManager;
	mp_menuManager = 0;

	delete mp_gameState;
	mp_gameState = 0;

	// Entities
	delete mp_player;
	mp_player = 0;

	delete mp_car;
	mp_car = 0;

	delete mp_gasoline;
	mp_gasoline = 0;

	for (std::vector<Zombie*>::iterator iter = mp_zombies.begin();
		iter != mp_zombies.end(); ++iter)
	{
		delete *iter;
		*iter = 0;
	}

	for (std::vector<Bullet*>::iterator iter = mp_bullets.begin();
		iter != mp_bullets.end(); ++iter)
	{
		delete *iter;
		*iter = 0;
	}
	
	gameWorld->DestroyBody(gameWorld->GetBodyList());
	delete gameWorld;
	gameWorld = 0;
}

bool
Game::Initialise()
{
	const int width = 800;
	const int height = 600;

	mp_backBuffer = new BackBuffer();
	if (!mp_backBuffer->Initialise(width, height))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}

	mp_backBuffer->SetClearColour(0x00, 0x00, 0x00);

	mp_inputHandler = new InputHandler();
	if (!mp_inputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	mp_levelManager = new LevelManager();
	if (!mp_levelManager->Initialise())
	{
		LogManager::GetInstance().Log("LevelManager Init Fail!");
		return (false);
	}

	mp_audioManager = new AudioManager();
	if (!mp_audioManager->Initialise())
	{
		LogManager::GetInstance().Log("AudioManager Init Fail!");
		return (false);
	}

	mp_menuManager = new MenuManager();
	if (!mp_menuManager->Initialise(mp_backBuffer))
	{
		LogManager::GetInstance().Log("MenuManager Init Fail!");
		return (false);
	}

	mp_gameState = new GameState();
	*mp_gameState = START;

	// Music
	mp_audioManager->PlayMusic();

	// Create Box2D world & build Level tiles
	LevelBuild();

	//TODO initialise objects

	//player
	//Sprite* playerSprite = mp_backBuffer->CreateSprite("assets\\Player\\idle\\survivor-idle_handgun_0.png");
	Sprite* playerSprite = mp_backBuffer->CreateSprite("assets\\DebuggingAssets\\player_test.png");
	mp_player = new Player();
	//mp_player->SetPositionX(((float)width / 2) - playerSprite->GetWidth() / 2);
	//mp_player->SetPositionY(((float)height / 2) - playerSprite->GetHeight() / 2);
	mp_player->SetPos(b2Vec2(((float)width / 2) - playerSprite->GetWidth() / 2, ((float)height / 2) - playerSprite->GetHeight() / 2));
	mp_player->Initialise(playerSprite, gameWorld, 0x0002, 0x0001 | 0x0003);


/*
	for (int i = 0; i < mi_totalBullets; ++i)
	{
		Bullet* bullet = new Bullet();
		Sprite* pBulletSprite = mp_backBuffer->CreateSprite("assets\\bullet.png");

		bullet->SetPos(mp_player->GetPos());
		bullet->Initialise(pBulletSprite, gameWorld);

		mp_bullets.push_back(bullet);
	}
*/

	//zombie
	int zCount = 3;
	for (int i = 0; i < zCount; i++)
	{
		SpawnZombie(700, 500);
	}

	// Car
	Sprite* carSprite = mp_backBuffer->CreateSprite("assets\\DebuggingAssets\\placeholder_L.png");
	mp_car = new Car();
	//mp_car->SetPositionX(((float)width / 3) - carSprite->GetWidth() / 2);
	//mp_car->SetPositionY(((float)height / 3) - carSprite->GetHeight() / 2);
	mp_car->SetPos(b2Vec2(((float)width / 3) - carSprite->GetWidth() / 2, ((float)height / 3) - carSprite->GetHeight() / 2));
	mp_car->Initialise(carSprite, gameWorld);

	// Gasoline
	Sprite* gasolineSprite = mp_backBuffer->CreateSprite("assets\\DebuggingAssets\\placeholder.png");
	mp_gasoline = new Gasoline();
	//mp_gasoline->SetPositionX((float)600 - gasolineSprite->GetWidth() / 2);
	//mp_gasoline->SetPositionY((float)200 - gasolineSprite->GetHeight() / 2);
	mp_gasoline->SetPos(b2Vec2((float)600 - gasolineSprite->GetWidth() / 2, (float)200 - gasolineSprite->GetHeight() / 2));
	mp_gasoline->Initialise(gasolineSprite, gameWorld);

	mi_lastTime = SDL_GetTicks();
	mf_lag = 0.0f;

	return (true);
}

bool
Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;
	// Box2D
	const int velocityIterations = 6;
	const int positionIterations = 2;

	assert(mp_inputHandler);
	mp_inputHandler->ProcessInput(*this);

	if (mb_looping)
	{
		int current = SDL_GetTicks();
		float deltaTime = (current - mi_lastTime) / 1000.0f;
		mi_lastTime = current;
		mf_executionTime += deltaTime;
		mf_lag += deltaTime;

		while (mf_lag >= stepSize )
		{
			if (*mp_gameState == PLAYING)
			{
				Process(stepSize);

				mp_audioManager->Update();

				gameWorld->Step(stepSize, velocityIterations, positionIterations);
			}
			else
			{
				//Process(0);

				//gameWorld->Step(0, 0, 0);
				if (mp_menuManager->GetQuit())
					mb_looping = false;
				else if (mp_menuManager->GetRestart())
				{
					//TODO Restart the game
					mp_menuManager->SetRestart(false);
					RestartGame();
					
				}
			}
			mf_lag -= stepSize;
			++mi_numUpdates;
		}

		Draw(*mp_backBuffer);
	}

	SDL_Delay(1);

	return (mb_looping);
}

void
Game::Process(float deltaTime)
{

	// Count total simulation time elapsed:
	mf_elapsedSeconds += deltaTime;

	// Frame Counter:
	if (mf_elapsedSeconds > 1)
	{
		mf_elapsedSeconds -= 1;
		mi_FPS = mi_frameCount;
		mi_frameCount = 0;
	}

	//TODO update
	// Player
	mp_player->Process(deltaTime);

	// Zombie	
	for (std::vector<Zombie*>::iterator iter = mp_zombies.begin();
		iter != mp_zombies.end(); ++iter)
	{
		Zombie* current = *iter;
		current->Process(deltaTime);
		//ZombieDetectPlayer(current);
	}

	if (mp_player->IsDead() && !mp_menuManager->GodMode())
		*mp_gameState = LOST;

	// Bullets
	for (std::vector<Bullet*>::iterator iter = mp_bullets.begin();
		iter != mp_bullets.end(); ++iter)
	{
		Bullet* current = *iter;
		current->Process(deltaTime);
	}

	// Car
	mp_car->Process(deltaTime);
	// Gasoline
	if (mp_gasoline != 0)
		mp_gasoline->Process(deltaTime);
	
	// Level
	LevelMove(); // Adjusts the Players position and Level tiles to allow the full map to scroll on screen
	mp_levelManager->Process(deltaTime);
	mp_bullets.erase(
		std::remove_if(
		mp_bullets.begin(), mp_bullets.end(),

		[](Bullet* b)
	{
		bool dead = b->IsDead();
		if (dead)
		{
			delete b;
			b = 0;
		}
		return dead;
	}), mp_bullets.end());
	
	mp_zombies.erase(
		std::remove_if(
		mp_zombies.begin(), mp_zombies.end(),
		[](Zombie* b)
	{
		bool dead = b->IsDead();
		if (dead)
		{
			delete b;
			b = 0;
		}
		return dead;
	}), mp_zombies.end());

	if (mp_gasoline != 0 && mp_gasoline->IsDead())
	{
		delete mp_gasoline;
		mp_gasoline = 0;
	}

	

	//Check if game is won
	for (b2ContactEdge* ce = mp_player->GetBox2DBody()->GetContactList(); ce != NULL; ce = ce->next)
	{

		Entity* tempCar = static_cast<Entity*>(ce->other->GetUserData());
		if (tempCar != NULL && tempCar->GetType() == CAR)
		{
			if (mp_player->HasGasoline())
				*mp_gameState = WON;
		}
	}
}

void
Game::Draw(BackBuffer& backBuffer)
{
	++mi_frameCount;
	backBuffer.Clear();

	if (*mp_gameState == PLAYING)
	{
		//TODO draw
		mp_levelManager->Draw(backBuffer);
		mp_car->Draw(backBuffer);
		if (mp_gasoline != 0)
			mp_gasoline->Draw(backBuffer);

		for (std::vector<Zombie*>::iterator iter = mp_zombies.begin();
			iter != mp_zombies.end(); ++iter)
		{
			Zombie* current = *iter;
			current->Draw(backBuffer);
		}

		for (std::vector<Bullet*>::iterator iter = mp_bullets.begin();
			iter != mp_bullets.end(); ++iter)
		{
			Bullet* current = *iter;
			current->Draw(backBuffer);
		}

		mp_player->Draw(backBuffer);
	}
	else
		mp_menuManager->DrawMenu(mp_gameState, mp_backBuffer);

	backBuffer.Present();
}

void
Game::Quit()
{
	mb_looping = false;
}

void
Game::VolumeAdjust(bool v)
{
	if (v)
	{
		mp_audioManager->VolumeUp();
	}
	else if (!v)
	{
		mp_audioManager->VolumeDown();
	}
}

void
Game::SpawnZombie(int x, int y)
{
	Zombie* tempZombie = new Zombie();

	Sprite* pZombieSprite = mp_backBuffer->CreateSprite("assets\\DebuggingAssets\\zombie_test.png");
	//pZombieSprite->SetX(static_cast<float>(x));
	//pZombieSprite->SetY(static_cast<float>(y));
	//tempZombie->SetPositionX(static_cast<float>(x));
	//tempZombie->SetPositionY(static_cast<float>(y));
	tempZombie->SetPos(b2Vec2(static_cast<float>(x), static_cast<float>(y)));
	tempZombie->Initialise(pZombieSprite, gameWorld, 0x0003, 0x0001 | 0x0002 | 0x0003 | 0x0004 | 0x0005);
	
	//tempZombie->ResetDirection();
	tempZombie->GetSteering()->SetTarget(&GetPlayer()->GetPos());
	tempZombie->GetSteering()->WanderOn();
	//tempZombie->GetSteering()->WallAvoidanceOn();
	//tempZombie->GetSteering()->SeekOn();
	//return tempZombie;
	mp_zombies.push_back(tempZombie);
}

void
Game::LevelBuild()
{
	const b2Vec2 m_gravity(0.0f, 0.0f);

	gameWorld = new b2World(m_gravity);

	mp_levelManager->CreateLevel(mp_backBuffer, gameWorld);
}

// Moves tiles when Player leaves the central bounds of the playable space. Allowing the level to scroll.
void
Game::LevelMove()
{
	const float speedTiles = 1.0f;
	const float distanceCorrection = 0.1f;
	const float distanceCorrectionXY = 0.165f;
	// Box2D scaling values used. * by 0.1 
	const int leftBoundX = 20;
	const int rightBoundX = 60;
	const int upperBoundY = 20;
	const int lowerBoundY = 40;

	// Adjust objects to counter act players movement
	// Reposition player and shift the level instead when the player moves out of the "moveable region"
	if (mp_player->GetBox2DBody()->GetPosition().x < leftBoundX && mp_player->GetBox2DBody()->GetPosition().y < upperBoundY)
	{
		LevelMoveIndividual(-speedTiles, -speedTiles);

		LevelMoveContainerAssets(distanceCorrectionXY, distanceCorrectionXY);

		mp_player->GetBox2DBody()->SetTransform(b2Vec2(leftBoundX + distanceCorrection, upperBoundY + distanceCorrection), mp_player->GetBox2DBody()->GetAngle());
	}
	else if (mp_player->GetBox2DBody()->GetPosition().y < upperBoundY && mp_player->GetBox2DBody()->GetPosition().x > rightBoundX)
	{
		LevelMoveIndividual(speedTiles, -speedTiles);

		LevelMoveContainerAssets(-distanceCorrectionXY, distanceCorrectionXY);

		mp_player->GetBox2DBody()->SetTransform(b2Vec2(rightBoundX - distanceCorrection, upperBoundY + distanceCorrection), mp_player->GetBox2DBody()->GetAngle());
	}
	else if (mp_player->GetBox2DBody()->GetPosition().x < leftBoundX && mp_player->GetBox2DBody()->GetPosition().y > lowerBoundY)
	{
		LevelMoveIndividual(-speedTiles, speedTiles);

		LevelMoveContainerAssets(distanceCorrectionXY, -distanceCorrectionXY);

		mp_player->GetBox2DBody()->SetTransform(b2Vec2(leftBoundX + distanceCorrection, lowerBoundY - distanceCorrection), mp_player->GetBox2DBody()->GetAngle());
	}
	else if (mp_player->GetBox2DBody()->GetPosition().y > lowerBoundY && mp_player->GetBox2DBody()->GetPosition().x > rightBoundX)
	{
		LevelMoveIndividual(speedTiles, speedTiles);

		LevelMoveContainerAssets(-distanceCorrectionXY, -distanceCorrectionXY);

		mp_player->GetBox2DBody()->SetTransform(b2Vec2(rightBoundX - distanceCorrection, lowerBoundY - distanceCorrection), mp_player->GetBox2DBody()->GetAngle());
	}

	// X axis
	else if (mp_player->GetBox2DBody()->GetPosition().x < leftBoundX)
	{
		LevelMoveIndividual(-speedTiles, 0);

		LevelMoveContainerAssets(distanceCorrectionXY, 0);

		mp_player->GetBox2DBody()->SetTransform(b2Vec2(leftBoundX + distanceCorrection, mp_player->GetBox2DBody()->GetPosition().y), mp_player->GetBox2DBody()->GetAngle());
	}
	else if (mp_player->GetBox2DBody()->GetPosition().x > rightBoundX)
	{
		LevelMoveIndividual(speedTiles, 0);

		LevelMoveContainerAssets(-distanceCorrectionXY, 0);

		mp_player->GetBox2DBody()->SetTransform(b2Vec2(rightBoundX - distanceCorrection, mp_player->GetBox2DBody()->GetPosition().y), mp_player->GetBox2DBody()->GetAngle());
	}
	// Y axis
	else if (mp_player->GetBox2DBody()->GetPosition().y < upperBoundY)
	{
		LevelMoveIndividual(0, -speedTiles);

		LevelMoveContainerAssets(0, distanceCorrectionXY);

		mp_player->GetBox2DBody()->SetTransform(b2Vec2(mp_player->GetBox2DBody()->GetPosition().x, upperBoundY + distanceCorrection), mp_player->GetBox2DBody()->GetAngle());
	}
	else if (mp_player->GetBox2DBody()->GetPosition().y > lowerBoundY)
	{
		LevelMoveIndividual(0, speedTiles);

		LevelMoveContainerAssets(0, -distanceCorrectionXY);

		mp_player->GetBox2DBody()->SetTransform(b2Vec2(mp_player->GetBox2DBody()->GetPosition().x, lowerBoundY - distanceCorrection), mp_player->GetBox2DBody()->GetAngle());
	}
	else
	{
		mp_levelManager->LevelStopTiles();
		mp_car->StopMove();
		if (mp_gasoline != 0)
		{
			mp_gasoline->StopMove();
		}
	}
}

void
Game::LevelMoveContainerAssets(float x, float y)
{
	for (int i = 0; i < mp_bullets.size(); ++i)
	{
		mp_bullets[i]->GetBox2DBody()->SetTransform(b2Vec2(mp_bullets[i]->GetBox2DBody()->GetPosition().x + x, mp_bullets[i]->GetBox2DBody()->GetPosition().y + y), mp_bullets[i]->GetBox2DBody()->GetAngle());
	}
	for (int i = 0; i < mp_zombies.size(); ++i)
	{
		mp_zombies[i]->GetBox2DBody()->SetTransform(b2Vec2(mp_zombies[i]->GetBox2DBody()->GetPosition().x + x, mp_zombies[i]->GetBox2DBody()->GetPosition().y + y), mp_zombies[i]->GetBox2DBody()->GetAngle());
	}
}

void
Game::LevelMoveIndividual(float x, float y)
{
	mp_levelManager->LevelMoveTiles(x, y);
	mp_car->Move(x, y);
	if (mp_gasoline != 0)
	{
		mp_gasoline->Move(x, y);
	}
}

Player*
Game::GetPlayer()
{
	return mp_player;
}


//void
//Game::ZombieDetectPlayer(Zombie* zombie)
//{
//	float currentRayAngle = zombie->GetRayAngle();
//
//	currentRayAngle += (360 / 1.0 / 60.0) * M_PI / 180;
//
//	//in Step() function
//	zombie->SetRayAngle(currentRayAngle); //one revolution every 1 second
//
//	//calculate points of ray
//	float rayLength = 100; //long enough to hit the walls
//	b2Vec2 p1 = zombie->GetPos(); //(zombie->GetPositionX(), zombie->GetPositionY()); //center of scene
//	b2Vec2 p2 = p1 + rayLength * b2Vec2(sinf(currentRayAngle), cosf(currentRayAngle));
//
//	b2RayCastInput input;
//	input.p1 = p1;
//	input.p2 = p2;
//	input.maxFraction = 1;
//
//	//check every fixture of every body to find closest
//	float closestFraction = 1; //start with end of line as p2
//	b2Vec2 intersectionNormal(0, 0);
//	for (b2Body* b = gameWorld->GetBodyList(); b; b = b->GetNext()) {
//		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {
//
//			b2RayCastOutput output;
//			if (!f->RayCast(&output, input, 0))
//				continue;
//			if (output.fraction < closestFraction) {
//				closestFraction = output.fraction;
//				intersectionNormal = output.normal;
//			}
//		}
//	}
//
//	b2Vec2 intersectionPoint = p1 + closestFraction * (p2 - p1);
//
//	float checkX = intersectionPoint.x - mp_player->GetPos().x;
//	float checkY = intersectionPoint.y - mp_player->GetPos().y;
//
//	if (checkX <= 30 && checkX >= -30 && checkY <= 30 && checkY >= -30)
//	{
//		zombie->GetSteering()->SetTarget(&GetPlayer()->GetPos());
//		zombie->ChangeStateAgro();
//	}
//}


void
Game::ShootGun()
{
	if (mp_player->GetAmmoCount() > 0)
	{
		if (!mp_menuManager->InfiniteAmmo())
			mp_player->DecreaseAmmo();

		Bullet* tempBullet = new Bullet();

		Sprite* pBulletSprite = mp_backBuffer->CreateSprite("assets\\bullet.png");

		tempBullet->SetPos(mp_player->GetPos());
		tempBullet->Initialise(pBulletSprite, gameWorld);

		tempBullet->Rotate(mp_player->GetBox2DBody()->GetAngle(), mp_player->GetWidth()/2, mp_player->GetHeight()/2);
		tempBullet->Move(mp_player->GetHeading().x, mp_player->GetHeading().y);

		mp_bullets.push_back(tempBullet);
		mp_audioManager->PlaySound(se_pSHOOT);
	}
	else
	{
		//TODO Play empty gun noise
		//mp_audioManager->PlaySound(se_pSHOOT);
	}
}

//
//void
//Game::ShootGun()
//{
//	mp_bullets[mi_currentBullet]->GetBox2DBody()->SetTransform(b2Vec2(mp_player->GetBox2DBody()->GetPosition().x, mp_player->GetBox2DBody()->GetPosition().y), 0);
//	mp_bullets[mi_currentBullet]->Rotate(mp_player->GetAngleX(), mp_player->GetAngleY(), mp_player->GetWidth()*0.5*0.1, mp_player->GetHeight()*0.5*0.1);
//	mp_bullets[mi_currentBullet]->Move(mp_player->GetAngleX(), mp_player->GetAngleY());
//
//	++mi_currentBullet;
//	if (mi_currentBullet == mi_totalBullets)
//	{
//		mi_currentBullet = 0;
//	}
//
//	mp_audioManager->PlaySound(se_pSHOOT);
//}

AudioManager*
Game::GetAudioManager()
{
	return mp_audioManager;
}

MenuManager*
Game::GetMenuManager()
{
	return mp_menuManager;
}

GameState*
Game::GetGameState()
{
	return mp_gameState;

}

void
Game::PauseGame()
{
	*mp_gameState = PAUSED;
}

void
Game::RestartGame()
{
	*mp_gameState = START;


	for (std::vector<Zombie*>::iterator iter = mp_zombies.begin();
		iter != mp_zombies.end(); ++iter)
	{
		delete *iter;
		*iter = 0;
	}

	mp_zombies.clear();

	for (std::vector<Bullet*>::iterator iter = mp_bullets.begin();
		iter != mp_bullets.end(); ++iter)
	{
		delete *iter;
		*iter = 0;
	}

	mp_bullets.clear();

	delete mp_car;
	mp_car = 0;

	delete mp_player;
	mp_player = 0;

	if (mp_gasoline != 0)
	{
		delete mp_gasoline;
		mp_gasoline = 0;
	}

	delete mp_levelManager;
	mp_levelManager = 0;

	gameWorld->DestroyBody(gameWorld->GetBodyList());
	delete gameWorld;
	gameWorld = 0;

	mp_levelManager = new LevelManager();
	if (!mp_levelManager->Initialise())
	{
		LogManager::GetInstance().Log("LevelManager Init Fail!");
		//return (false);
	}

	
	LevelBuild();

	

	// Car
	Sprite* carSprite = mp_backBuffer->CreateSprite("assets\\DebuggingAssets\\placeholder_L.png");
	mp_car = new Car();
	//mp_car->SetPositionX(((float)width / 3) - carSprite->GetWidth() / 2);
	//mp_car->SetPositionY(((float)height / 3) - carSprite->GetHeight() / 2);
	mp_car->SetPos(b2Vec2(((float)800 / 3) - carSprite->GetWidth() / 2, ((float)600 / 3) - carSprite->GetHeight() / 2));
	mp_car->Initialise(carSprite, gameWorld);

	

	//player
	//Sprite* playerSprite = mp_backBuffer->CreateSprite("assets\\Player\\idle\\survivor-idle_handgun_0.png");
	Sprite* playerSprite = mp_backBuffer->CreateSprite("assets\\DebuggingAssets\\player_test.png");
	mp_player = new Player();
	//mp_player->SetPositionX(((float)width / 2) - playerSprite->GetWidth() / 2);
	//mp_player->SetPositionY(((float)height / 2) - playerSprite->GetHeight() / 2);
	mp_player->SetPos(b2Vec2(((float)800 / 2) - playerSprite->GetWidth() / 2, ((float)600 / 2) - playerSprite->GetHeight() / 2));
	mp_player->Initialise(playerSprite, gameWorld, 0x0002, 0x0001 | 0x0003);

	//zombie
	int zCount = 3;
	for (int i = 0; i < zCount; i++)
	{
		SpawnZombie(100, 100);
	}

	// Gasoline
	Sprite* gasolineSprite = mp_backBuffer->CreateSprite("assets\\DebuggingAssets\\placeholder.png");
	mp_gasoline = new Gasoline();
	//mp_gasoline->SetPositionX((float)600 - gasolineSprite->GetWidth() / 2);
	//mp_gasoline->SetPositionY((float)200 - gasolineSprite->GetHeight() / 2);
	mp_gasoline->SetPos(b2Vec2((float)600 - gasolineSprite->GetWidth() / 2, (float)200 - gasolineSprite->GetHeight() / 2));
	mp_gasoline->Initialise(gasolineSprite, gameWorld);
}