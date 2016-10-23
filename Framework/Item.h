#ifndef __ITEM_H__
#define __ITEM_H__
#include "Entity.h"

#include "Box2D.h"

// Forward Declarations
class BackBuffer;
class Sprite;

class Item :
	public Entity
{
	// Member Methods:
public:
	Item();
	~Item();

	bool Initialise(Sprite* sprite, b2World* gameWorld);
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	b2Body* GetBox2DBody();
	void Move(float x, float y);
	void StopMove();

	int  GetQuantity();
	void SetQuantity(int q);

protected:
private:
	void box2DBodyCreate(b2World* gameWorld, float width, float height);

	// Member Data:
public:

protected:
	int mi_quantity;

private:

	b2Body* mp_body;
};

#endif //__ITEM_H__