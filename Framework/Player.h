#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "Character.h"
#include "Ammo.h"
#include "Bullet.h"
#include "AnimatedSprite.h"

#include <Box2D.h>
#include <vector>

class Player :
	public Character
{
	// Member Methods:
public:
	Player();
	~Player();
	//bool Initialise(Sprite* animatedSprite, b2World* gameWorld);
	//void Draw(BackBuffer& backBuffer);
	//bool Initialise(AnimatedSprite* animatedSprite, b2World* gameWorld);

	void Process(float deltaTime);

	int GetAmmoCount();
	void DecreaseAmmo();

protected:

private:
	Player(const Player& Player);
	Player& operator = (const Player& Player);

	// Member Data:
public:
	bool HasGasoline();
	void PickUpGasoline();
	void PickUpAmmo(Ammo& ammo);
	void Rotate(float x, float y);
	//void Move(float x, float y);
	//void Rotate(float x, float y);
	//void StopMove();
	float GetAngleX();
	float GetAngleY();

protected:
	bool mb_hasGasoline;
	int mi_ammoCount;
	

	float mf_angleX;
	float mf_angleY;
	//b2Body* mp_dynamicPlayerBody;
private:

};

#endif //__PLAYER_H__