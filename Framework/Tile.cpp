// This include:
#include "Tile.h"

// Local includes:
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "BackBuffer.h"

// Library includes:
#include <cassert>

Tile::Tile()
: mp_body(0)
{
	m_type = EMPTY;
}

Tile::~Tile()
{
	delete mp_sprite;
	mp_sprite = 0;

	if (m_type == BOUNDARY)
		mp_body->DestroyFixture(mp_body->GetFixtureList());
}

bool
Tile::Initialise(Sprite* sprite)
{
	assert(sprite);
	mp_sprite = sprite;



	return (true);
}

bool
Tile::Initialise(Sprite* sprite, b2World* gameWorld, bool collision)
{
	assert(sprite);
	mp_sprite = sprite;
	mi_width = mp_sprite->GetWidth();
	mi_height = mp_sprite->GetHeight();
	box2DBodyCreate(gameWorld, collision, static_cast<float>(sprite->GetWidth()) / 2, static_cast<float>(sprite->GetHeight()) / 2); // Height and Width taken as half lengths object width = 2*given width

	return (true);
}

// width and height are handled as half values in Box2D
void
Tile::box2DBodyCreate(b2World* gameWorld, bool collision, float mf_spriteWidth, float mf_spriteHeight)
{
	b2BodyDef m_entityBodyDef;
	// Origin is at center. Must be offset by sprite width/height
	m_entityBodyDef.type = b2_kinematicBody;
	m_entityBodyDef.position.Set((mv_pos.x + mf_spriteWidth) * mf_worldScaleB2D, (mv_pos.y + mf_spriteHeight) * mf_worldScaleB2D);
	m_entityBodyDef.angle = 0;
	m_entityBodyDef.allowSleep = true;
	m_entityBodyDef.gravityScale = 0.0f;

	mp_body = gameWorld->CreateBody(&m_entityBodyDef);
	mp_body->SetUserData(this);

	// Create collision shape if object should collide
	if (collision)
	{
		b2PolygonShape boxShape;
		// Creates width and height of box (half measures used), given the center, angle
		boxShape.SetAsBox(mf_spriteWidth * mf_worldScaleB2D, mf_spriteHeight * mf_worldScaleB2D);

		b2FixtureDef boxFixtureDef;
		boxFixtureDef.shape = &boxShape;
		boxFixtureDef.friction = 100;
		boxFixtureDef.filter.categoryBits = 0x0001;
		boxFixtureDef.filter.maskBits = 0x0001 | 0x0002 | 0x0003 | 0x0004;

		mp_body->CreateFixture(&boxFixtureDef);

		m_type = BOUNDARY;
	}
}

void
Tile::Process(float deltaTime)
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
Tile::Draw(BackBuffer& backBuffer)
{
	assert(mp_sprite);
	
	if (mv_pos.x > 0 - mi_width*2 && mv_pos.x < 800 &&
		mv_pos.y > 0 - mi_height*2 && mv_pos.y < 600)
	{
		mp_sprite->Draw(backBuffer);
	}
}

void
Tile::Move(float x, float y)
{
	float m = sqrt(pow(x, 2) + pow(y, 2));
	float normX = x / m;
	float normY = y / m;

	mp_body->SetLinearVelocity(b2Vec2(-normX * 10, -normY * 10));
}

b2Body*
Tile::GetBox2DBody()
{
	return mp_body;
}