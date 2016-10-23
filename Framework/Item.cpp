
// This includes:
#include "Item.h"

// Local includes:
#include "Sprite.h"

Item::Item()
: mi_quantity(0)
, mp_body(nullptr)
{
}

Item::~Item()
{
}

bool
Item::Initialise(Sprite* sprite, b2World* gameWorld)
{
	assert(sprite);
	mp_sprite = sprite;
	mi_width = sprite->GetWidth();
	mi_height = sprite->GetHeight();

	box2DBodyCreate(gameWorld, static_cast<float>(sprite->GetWidth()) / 2, static_cast<float>(sprite->GetHeight()) / 2); // Height and Width taken as half lengths object width = 2*given width

	return (true);
}

void
Item::box2DBodyCreate(b2World* gameWorld, float mf_spriteWidth, float mf_spriteHeight)
{
	b2BodyDef m_entityBodyDef;
	m_entityBodyDef.type = b2_kinematicBody;
	// Origin is at center. Must be offset by sprite width/height
	m_entityBodyDef.position.Set((mv_pos.x + mf_spriteWidth) * mf_worldScaleB2D, (mv_pos.y + mf_spriteHeight) * mf_worldScaleB2D);
	m_entityBodyDef.angle = 0;
	m_entityBodyDef.allowSleep = false;
	m_entityBodyDef.gravityScale = 1.0f;

	mp_body = gameWorld->CreateBody(&m_entityBodyDef);
}

void
Item::Process(float deltaTime)
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
Item::Draw(BackBuffer& backBuffer)
{
	Entity::Draw(backBuffer);
}

void
Item::Move(float x, float y)
{
	b2Vec2 norm(-x, -y);
	norm.Normalize();

	mp_body->SetLinearVelocity(10 * norm);
}

void
Item::StopMove()
{
	mp_body->SetLinearVelocity(b2Vec2(0, 0));
}

b2Body*
Item::GetBox2DBody()
{
	return mp_body;
}

int
Item::GetQuantity()
{
	return mi_quantity;
}

void
Item::SetQuantity(int q)
{
	mi_quantity = q;
}