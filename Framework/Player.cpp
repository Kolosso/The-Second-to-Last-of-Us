
#include "Player.h"
#include "Bullet.h"
#include "AnimatedSprite.h"
#include <ostream>
#include <iostream>

//#include "Vector2D.h"


Player::Player()
: mb_hasGasoline(false)
, mi_ammoCount(6)
, mf_angleX(0)
, mf_angleY(0)
{
	m_type = PLAYER;
	mv_heading = b2Vec2(1,0);
}


Player::~Player()
{

}

void
Player::Process(float deltaTime)
{
	for (b2ContactEdge* ce = mp_body->GetContactList(); ce != NULL; ce = ce->next)
	{

		Entity* tempGas = static_cast<Entity*>(ce->other->GetUserData());
		if (tempGas != NULL && tempGas->GetType() == GAS)
		{
			mb_hasGasoline = true;
			tempGas->SetDead(true);
		}
	}
	Character::Process(deltaTime);
}

bool 
Player::HasGasoline()
{
	return mb_hasGasoline;
}

void 
Player::PickUpGasoline()
{
	mb_hasGasoline = true;
}

void 
Player::PickUpAmmo(Ammo& ammo)
{
	mi_ammoCount += ammo.GetQuantity();
	//TODO Delete the ammo once picked up?
}

void
Player::Rotate(float x, float y)
{
	Character::Rotate(x, y);
	mv_heading = b2Vec2(x, y);//b2Vec2(cos(mp_body->GetAngle()), sin(mp_body->GetAngle()));
	mf_angleX = x;
	mf_angleY = y;
}

float
Player::GetAngleX()
{
	return mf_angleX;
}

float
Player::GetAngleY()
{
	return mf_angleY;
}

int
Player::GetAmmoCount()
{
	return mi_ammoCount;
}

void
Player::DecreaseAmmo()
{
	--mi_ammoCount;
}