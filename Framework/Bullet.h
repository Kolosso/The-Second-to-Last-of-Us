#ifndef __BULLET_H__
#define __BULLET_H__
#include "Entity.h"

// Library includes
#include <Box2D.h>

class Bullet :
	public Entity
{
	// Member Methods:
public:
	Bullet();
	~Bullet();

	bool Initialise(Sprite* sprite);
	bool Initialise(Sprite* sprite, b2World* gameWorld);

	void Process(float deltaTime);

	float GetVelocityX();
	float GetVelocityY();

	void SetVelocityX(float velocityX);
	void SetVelocityY(float velocityY);


	b2Body* GetBox2DBody();
	void Move(float x, float y);
	void Rotate(float x, float y, float offsetX, float offsetY);
	void Rotate(float angle, float offsetX, float offsetY);
protected:

private:
	void box2DBodyCreate(b2World* gameWorld, float width, float height);
	// Member Data:
public:

protected:
	float mf_velocityX;
	float mf_velocityY;

	b2Body*   mp_body;

private:

};

#endif //__BULLET_H__