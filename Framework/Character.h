#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "Entity.h"

// Library includes
#include <Box2D.h>
#include "SteeringBehaviours.h"

// Forward Declarations:
class BackBuffer;
class Sprite;

class Character 
	: public Entity
{
	//Member Methods:
public:
	Character();
	~Character();

	bool Initialise(Sprite* sprite);
	bool Initialise(Sprite* sprite, b2World* gameWorld, uint16 categoryBits, uint16 maskBits);

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	float GetHorizontalVelocity();
	void SetHorizontalVelocity(float x);

	float GetVerticalVelocity();
	void SetVerticalVelocity(float y);

	// Box2D
	b2Body* GetBox2DBody();
	void Move(float x, float y);
	void Move(b2Vec2& pos);
	void Move(b2Vec2& pos, float speed);
	void MoveToPoint(b2Vec2& pos);
	void Rotate(float x, float y);
	void Rotate(b2Vec2 heading);
	void StopMove();
	void Damage(int dmg);
	float GetMaxSpeed();
	b2Vec2& GetVelocity();
	b2Vec2& GetHeading();
	b2Vec2& GetSide();
	SteeringBehaviours* GetSteering();
	b2World* GetWorld();
	float GetMaxForce();
protected:

private:
	Character(const Character& Character);
	Character& operator=(const Character& Character);

	void box2DBodyCreate(b2World* gameWorld, float width, float height, uint16 categoryBits, uint16 maskBits);

	//Member Data:
public:

protected:
	float mf_velocityX;
	float mf_velocityY;

	int mi_spriteWidth;
	int mi_spriteHeight;

	int mi_hp;

	// Collision body
	b2Body*   mp_body;
	
	SteeringBehaviours* mp_steering;
	b2Vec2 mv_velocity;
	b2Vec2 mv_heading;
		//vector perpendiculer to the heading vector
	b2Vec2 mv_side;
	b2World* mp_gameWorld;
	float mf_mass;
	float mf_maxSpeed;
	float mf_maxForce;

private:
};

#endif //__CHARACTER_H__
