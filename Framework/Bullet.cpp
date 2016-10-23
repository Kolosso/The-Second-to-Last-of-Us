
#define _USE_MATH_DEFINES

#include "Bullet.h"
#include "Sprite.h"

#include <math.h>

Bullet::Bullet()
	: Entity()
, mf_velocityX(0)
, mf_velocityY(0)
, mp_body(nullptr)
{
	m_type = BULLET;
}

Bullet::~Bullet()
{
	mp_body->DestroyFixture(mp_body->GetFixtureList());
}

bool
Bullet::Initialise(Sprite* sprite)
{
	Entity::Initialise(sprite);

	return (true);
}

bool
Bullet::Initialise(Sprite* sprite, b2World* gameWorld)
{
	assert(sprite);
	mp_sprite = sprite;
	mi_width = sprite->GetWidth();
	mi_height = sprite->GetHeight();

	box2DBodyCreate(gameWorld, static_cast<float>(sprite->GetWidth()) / 2, static_cast<float>(sprite->GetHeight()) / 2); // Height and Width taken as half lengths object width = 2*given width

	return (true);
}

void
Bullet::Process(float deltaTime)
{
	//Entity::Process(deltaTime);

	for (b2ContactEdge* ce = mp_body->GetContactList(); ce != NULL; ce = ce->next)
	{

		Entity* tempWall = static_cast<Entity*>(ce->other->GetUserData());
		if (tempWall != NULL && tempWall->GetType() == BOUNDARY || tempWall->GetType() == CAR)
		{
			mb_dead = true;
		}
	}


	if (mp_body != 0)
	{
		// Base position on collision body. Collision body position has central origin.
		//Position must be adjusted based on half sprite width / height to account for sprite 0, 0 in top left
		mv_pos.x = static_cast<float>((mp_body->GetPosition().x * (mf_worldScaleSprite)) - (mp_sprite->GetWidth() / 2));
		mv_pos.y = static_cast<float>((mp_body->GetPosition().y * (mf_worldScaleSprite)) - (mp_sprite->GetHeight() / 2));
	}

	mp_sprite->SetPos(mv_pos);
}

float Bullet::GetVelocityX()
{
	return mf_velocityX;
}

float Bullet::GetVelocityY()
{
	return mf_velocityY;
}


void Bullet::SetVelocityX(float velocityX)
{
	mf_velocityX = velocityX;
}

void Bullet::SetVelocityY(float velocityY)
{
	mf_velocityY = velocityY;
}


b2Body*
Bullet::GetBox2DBody()
{
	return mp_body;
}

// width and height are handled as half values in Box2D
void
Bullet::box2DBodyCreate(b2World* gameWorld, float mf_spriteWidth, float mf_spriteHeight)
{
	b2BodyDef m_entityBodyDef;
	m_entityBodyDef.type = b2_dynamicBody;
	// Bullet type set to true. More precise collisions
	m_entityBodyDef.bullet = true;
	// Origin is at center. Must be offset by sprite width/height
	m_entityBodyDef.position.Set((mv_pos.x + mf_spriteWidth) * mf_worldScaleB2D, (mv_pos.y + mf_spriteHeight) * mf_worldScaleB2D);
	m_entityBodyDef.angle = 0;
	m_entityBodyDef.allowSleep = false;
	m_entityBodyDef.gravityScale = 1.0f;

	mp_body = gameWorld->CreateBody(&m_entityBodyDef);
	mp_body->SetUserData(this);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(mf_spriteWidth * mf_worldScaleB2D, mf_spriteHeight * mf_worldScaleB2D);

	b2FixtureDef boxFixtureDef;
	boxFixtureDef.shape = &boxShape;
	boxFixtureDef.friction = 0;
	boxFixtureDef.density = 1;
	boxFixtureDef.filter.categoryBits = 0x0004;
	boxFixtureDef.filter.maskBits = 0x0001 | 0x0003;

	mp_body->CreateFixture(&boxFixtureDef);
	//mp_body->SetLinearDamping(5); // Gradually slows players movement. Prevents him from sliding
}

void
Bullet::Move(float x, float y)
{
	b2Vec2 norm = b2Vec2(x, y);
	norm.Normalize();

	//mp_body->ApplyLinearImpulse(b2Vec2(normX * mf_worldScaleB2D, normY * mf_worldScaleB2D), mp_body->GetWorldCenter(), true);
	//TODO: Make linear velocity to work properly?
	mp_body->SetLinearVelocity(30 * norm);
}

void
Bullet::Rotate(float x, float y, float offsetX, float offsetY)
{
	float angle = b2Atan2(y, x);
	//b2Vec2 temp (0, 0);
	//mp_body->SetTransform(mp_body->GetPosition(), angle);
	b2Vec2 offset;
	offset.x = offsetX;
	offset.y = offsetY;
	mp_body->SetTransform(mp_body->GetPosition() + mf_worldScaleB2D * offset, angle);
	mp_sprite->SetAngle(angle * 180 / M_PI);
}

void
Bullet::Rotate(float angle, float offsetX, float offsetY)
{
	//b2Vec2 temp (0, 0);
	//mp_body->SetTransform(mp_body->GetPosition(), angle);
	b2Vec2 offset;
	offset.x = offsetX;
	offset.y = offsetY;
	mp_body->SetTransform(mp_body->GetPosition() + mf_worldScaleB2D * offset, angle);
	mp_sprite->SetAngle(angle * 180 / M_PI);
}