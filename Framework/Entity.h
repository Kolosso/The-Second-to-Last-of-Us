// COMP710 GP 2D Framework
#ifndef __ENTITY_H__
#define __ENTITY_H__
#include <Box2D/Common/b2Math.h>

// Forward Declarations:

#include "EntityType.h"

class BackBuffer;
class Sprite;

class Entity
{
	//Member Methods:
public:
	Entity();
	~Entity();

	bool Initialise(Sprite* sprite);

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	bool IsCollidingWith(Entity& e);

	//float GetPositionX();
	//float GetPositionY();
	Sprite* GetSprite();
	int GetWidth();
	int GetHeight();

	//void SetPositionX(float x);
	//void SetPositionY(float y);
	void SetWidth(int width);
	void SetHeight(int height);

	b2Vec2& GetPos();
	void SetPos(b2Vec2 pos);

	void SetDead(bool b);
	bool IsDead();

	EntityType GetType();

protected:

private:
	Entity(const Entity& entity);
	Entity& operator=(const Entity& entity);

	//Member Data:
public:

protected:
	Sprite* mp_sprite;

	//float mf_x;
	//float mf_y;

	int mi_width;
	int mi_height;

	bool mb_dead;

	EntityType m_type;

	// Game resolution example:  800x600. B2D will work at 80x60
	float mf_worldScaleB2D;    // Used to adjust the positioning of the Collision body to match game resolution space 
	float mf_worldScaleSprite; // Used to position sprite correctly in world space / game resolution space

	b2Vec2 mv_pos;

private:
};

#endif //__ENTITY_H__
