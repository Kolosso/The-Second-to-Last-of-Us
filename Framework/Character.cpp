// COMP710 GP 2D Framework
#define _USE_MATH_DEFINES

// This include:
#include "Character.h"

// Local includes:
#include "Sprite.h"
#include "BackBuffer.h"
#include "Entity.h"

// Library includes:
#include <cassert>
#include <iostream>
#include <math.h>

Character::Character()
: Entity()
, mf_velocityX(0.0f)
, mf_velocityY(0.0f)
, mi_spriteWidth(0)
, mi_spriteHeight(0)
, mi_hp(1)
, mp_body(0)
, mp_steering(nullptr)
, mp_gameWorld(nullptr)
, mf_mass(0)
, mf_maxSpeed(0)
, mf_maxForce(0)
{
	mb_dead = false;
}

Character::~Character()
{
	delete mp_steering;
	mp_steering = 0;

	mp_body->DestroyFixture(mp_body->GetFixtureList());
}

//TODO initialise with AnimatedSprite

bool
Character::Initialise(Sprite* sprite)
{
	Entity::Initialise(sprite);

	return (true);
}

bool
Character::Initialise(Sprite* sprite, b2World* gameWorld, uint16 categoryBits, uint16 maskBits)
{
	assert(sprite);
	mp_sprite = sprite;
	mp_gameWorld = gameWorld;
	mi_width = sprite->GetWidth();
	mi_height = sprite->GetHeight();

	box2DBodyCreate(gameWorld, static_cast<float>(sprite->GetWidth()) / 2, static_cast<float>(sprite->GetHeight()) / 2, categoryBits, maskBits); // Height and Width taken as half lengths object width = 2*given width

	return (true);
}

// width and height are handled as half values in Box2D
void
Character::box2DBodyCreate(b2World* gameWorld, float mf_spriteWidth, float mf_spriteHeight, uint16 categoryBits, uint16 maskBits)
{
	b2BodyDef m_entityBodyDef;
	m_entityBodyDef.type = b2_dynamicBody;
	// Origin is at center. Must be offset by sprite width/height
	m_entityBodyDef.position.Set((mv_pos.x + mf_spriteWidth) * mf_worldScaleB2D, (mv_pos.y + mf_spriteHeight) * mf_worldScaleB2D);
	m_entityBodyDef.angle = 0;
	m_entityBodyDef.allowSleep = false;
	m_entityBodyDef.gravityScale = 1.0f;

	mp_body = gameWorld->CreateBody(&m_entityBodyDef);
	mp_body->SetUserData(this);

	b2CircleShape dynamicCircle;
	dynamicCircle.m_p.Set(0.0f, 0.0f);
	dynamicCircle.m_radius = mf_spriteWidth * mf_worldScaleB2D;

	b2FixtureDef boxFixtureDef;
	boxFixtureDef.shape = &dynamicCircle;
	boxFixtureDef.friction = 0;
	boxFixtureDef.density = 1;
	boxFixtureDef.filter.categoryBits = categoryBits;
	boxFixtureDef.filter.maskBits = maskBits;

	mp_body->CreateFixture(&boxFixtureDef);
	//mp_body->SetLinearDamping(5); // Gradually slows players movement. Prevents him from sliding
}

void
Character::Process(float deltaTime)
{
	//Entity::Process(deltaTime);
	
	if (mp_body != 0)
	{
		// Base position on collision body. Collision body position has central origin.
		//Position must be adjusted based on half sprite width / height to account for sprite 0, 0 in top left
		mv_pos.x = static_cast<float>((mp_body->GetPosition().x * (mf_worldScaleSprite)) - (mp_sprite->GetWidth() / 2));
		mv_pos.y = static_cast<float>((mp_body->GetPosition().y * (mf_worldScaleSprite)) - (mp_sprite->GetHeight() / 2));
	}

	mp_sprite->SetPos(mv_pos);
	mp_sprite->SetAngle(mp_body->GetAngle() * 180.0f / static_cast<float>(M_PI));
}

void
Character::Draw(BackBuffer& backBuffer)
{
	Entity::Draw(backBuffer);
}

void
Character::Damage(int dmg)
{
	mi_hp -= dmg;
	if (mi_hp <= 0)
		SetDead(true);
}


float
Character::GetHorizontalVelocity()
{
	return (mf_velocityX);
}

float
Character::GetVerticalVelocity()
{
	return (mf_velocityY);
}

void
Character::SetHorizontalVelocity(float x)
{
	mf_velocityX = x;
}

void
Character::SetVerticalVelocity(float y)
{
	mf_velocityY = y;
}

b2Body*
Character::GetBox2DBody()
{
	return mp_body;
}

void
Character::Move(float x, float y)
{
	b2Vec2 heading(x, y);
	heading.Normalize();

	//mp_body->ApplyLinearImpulse(b2Vec2(normX * mf_worldScaleB2D, normY * mf_worldScaleB2D), mp_body->GetWorldCenter(), true);
	//TODO: Make linear velocity to work properly?
	mp_body->SetLinearVelocity(10 * heading);
}

void
Character::Move(b2Vec2& pos)
{
	b2Vec2 heading = pos;
	heading.Normalize();

	//mp_body->ApplyLinearImpulse(b2Vec2(normX * mf_worldScaleB2D, normY * mf_worldScaleB2D), mp_body->GetWorldCenter(), true);
	//TODO: Make linear velocity to work properly?
	mp_body->SetLinearVelocity(10 * heading);
}

void
Character::Move(b2Vec2& pos, float speed)
{
	b2Vec2 heading = pos;
	heading.Normalize();

	//mp_body->ApplyLinearImpulse(b2Vec2(normX * mf_worldScaleB2D, normY * mf_worldScaleB2D), mp_body->GetWorldCenter(), true);
	//TODO: Make linear velocity to work properly?
	mp_body->SetLinearVelocity(speed * heading);
}

void
Character::MoveToPoint(b2Vec2& pos)
{
	b2Vec2 heading = pos;

	//mp_body->ApplyLinearImpulse(b2Vec2(normX * mf_worldScaleB2D, normY * mf_worldScaleB2D), mp_body->GetWorldCenter(), true);
	//TODO: Make linear velocity to work properly?
	mp_body->SetLinearVelocity(heading);
}

void
Character::Rotate(float x, float y)
{
	float angle = b2Rot(b2Atan2(y, x)).GetAngle();
	mp_body->SetTransform(mp_body->GetPosition(), angle);

	// Rotation physics body why retaining position.
	// Sprite rotation should be based on the angle of the physics body.
	mp_sprite->SetAngle(mp_body->GetAngle() * 180.0f / static_cast<float>(M_PI));
}

void
Character::Rotate(b2Vec2 heading)
{
	float angle = b2Rot(b2Atan2(heading.y, heading.x)).GetAngle();
	mp_body->SetTransform(mp_body->GetPosition(), angle);

	// Rotation physics body why retaining position.
	// Sprite rotation should be based on the angle of the physics body.
	mp_sprite->SetAngle(mp_body->GetAngle() * 180.0f / static_cast<float>(M_PI));
}

void
Character::StopMove()
{
	mp_body->SetLinearVelocity(b2Vec2(0, 0));
}

float
Character::GetMaxSpeed()
{
	return mf_maxSpeed;
}

b2Vec2&
Character::GetVelocity()
{
	return mv_velocity;
}

b2Vec2&
Character::GetHeading()
{
	return mv_heading;
}

b2Vec2&
Character::GetSide()
{
	return mv_side;
}

SteeringBehaviours*
Character::GetSteering()
{
	return mp_steering;
}

b2World*
Character::GetWorld()
{
	return mp_gameWorld;
}

float 
Character::GetMaxForce()
{
	return mf_maxForce;
}
