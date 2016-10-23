#ifndef __TILE_H__
#define __TILE_H__

// Local includes:
#include "Entity.h"

// Library includes:
#include <Box2D.h>

// Forward Declarations:
class BackBuffer;
class Sprite;
class AnimatedSprite;

class Tile : public Entity
{

	// Member methods:
public:
	Tile();
	~Tile();

	bool Initialise(Sprite* sprite);
	bool Initialise(Sprite* sprite, b2World* gameWorld, bool collision);

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void Move(float x, float y);

	// PositionX
	//float GetPositionX();
	//void  SetPositionX(float x);
	// PositionY
	//float GetPositionY();
	//void  SetPositionY(float y);

	// Box2D
	b2Body* GetBox2DBody();

protected:

private:
	Tile(const Tile& tile);
	Tile& operator=(const Tile& tile);

	void box2DBodyCreate(b2World* gameWorld, bool collision, float width, float height);

	// Member data:
public:

protected:

	// Collision body
	b2Body* mp_body;

private:

};

#endif // __TILE_H__