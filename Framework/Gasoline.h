#ifndef __GASOLINE_H__
#define __GASOLINE_H__

#include "Item.h"

class Gasoline : public Entity
{
	// Memember Data
public:
	Gasoline();
	~Gasoline();

	bool Initialise(Sprite* sprite, b2World* gameWorld);

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	b2Body* GetBox2DBody();
	void Move(float x, float y);
	void StopMove();

protected:
private:
	void box2DBodyCreate(b2World* gameWorld, float width, float height);
	// Memeber Variables
public:
protected:
private:
	b2Body* mp_body;
};


#endif // __GASOLINE_H__
