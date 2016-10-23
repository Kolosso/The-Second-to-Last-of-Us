
// This includes:
#include "Gasoline.h"

// Local includes:
#include "Sprite.h"

Gasoline::Gasoline()
	: mp_body(0)
{
	m_type = GAS;
}

Gasoline::~Gasoline()
{
	mp_body->DestroyFixture(mp_body->GetFixtureList());
}

bool
Gasoline::Initialise(Sprite* sprite, b2World* gameWorld)
{
	assert(sprite);
	mp_sprite = sprite;
	mi_width = sprite->GetWidth();
	mi_height = sprite->GetHeight();

	box2DBodyCreate(gameWorld, static_cast<float>(sprite->GetWidth()) / 2, static_cast<float>(sprite->GetHeight()) / 2); // Height and Width taken as half lengths object width = 2*given width

	return (true);
}

void
Gasoline::box2DBodyCreate(b2World* gameWorld, float mf_spriteWidth, float mf_spriteHeight)
{
	b2BodyDef m_entityBodyDef;
	m_entityBodyDef.type = b2_kinematicBody;
	// Origin is at center. Must be offset by sprite width/height
	m_entityBodyDef.position.Set((mv_pos.x + mf_spriteWidth) * mf_worldScaleB2D, (mv_pos.y + mf_spriteHeight) * mf_worldScaleB2D);
	m_entityBodyDef.angle = 0;
	m_entityBodyDef.allowSleep = false;
	m_entityBodyDef.gravityScale = 1.0f;

	mp_body = gameWorld->CreateBody(&m_entityBodyDef);
	mp_body->SetUserData(this);



	b2PolygonShape boxShape;
	// Creates width and height of box (half measures used), given the center, angle
	boxShape.SetAsBox(mf_spriteWidth * mf_worldScaleB2D, mf_spriteHeight * mf_worldScaleB2D);

	b2FixtureDef boxFixtureDef;
	boxFixtureDef.shape = &boxShape;
	boxFixtureDef.friction = 100;
	boxFixtureDef.filter.categoryBits = 0x0005;
	boxFixtureDef.filter.maskBits = 0x0001 | 0x0002 | 0x0003;

	mp_body->CreateFixture(&boxFixtureDef);
}

void
Gasoline::Process(float deltaTime)
{

	if (mp_body != 0)
	{
		// Base position on collision body. Collision body position has central origin.
		//Position must be adjusted based on half sprite width / height to account for sprite 0, 0 in top left
		mv_pos.x = static_cast<float>((mp_body->GetPosition().x * (mf_worldScaleSprite)) - (mp_sprite->GetWidth() / 2));
		mv_pos.y = static_cast<float>((mp_body->GetPosition().y * (mf_worldScaleSprite)) - (mp_sprite->GetHeight() / 2));
	}

	mp_sprite->SetPos(mv_pos);
}

void
Gasoline::Draw(BackBuffer& backBuffer)
{
	Entity::Draw(backBuffer);
}

b2Body*
Gasoline::GetBox2DBody()
{
	return mp_body;
}

void
Gasoline::Move(float x, float y)
{
	b2Vec2 norm = b2Vec2(-x, -y);
	norm.Normalize();

	mp_body->SetLinearVelocity(10 * norm);
}

void
Gasoline::StopMove()
{
	mp_body->SetLinearVelocity(b2Vec2(0, 0));
}