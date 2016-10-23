// COMP710 GP 2D Framework

// This include:
#include "Entity.h"

// Local includes:
#include "Sprite.h"
#include "BackBuffer.h"

// Library includes:
#include <cassert>

Entity::Entity()
: mp_sprite(0)
, mi_width(0)
, mi_height(0)
, mb_dead(false)
, m_type()
, mf_worldScaleB2D(0.1f)
, mf_worldScaleSprite(10.0f)
{
}

Entity::~Entity()
{
	delete mp_sprite;
	mp_sprite = 0;
}

bool
Entity::Initialise(Sprite* sprite)
{
	assert(sprite);
	mp_sprite = sprite;
	mi_width = mp_sprite->GetWidth();
	mi_height = mp_sprite->GetHeight();
	return (true);
}

void
Entity::Process(float deltaTime)
{
	mp_sprite->SetPos(mv_pos);

	//Update position etc
}

void
Entity::Draw(BackBuffer& backBuffer)
{
	assert(mp_sprite);
	if (mv_pos.x > 0 - mi_width * 2 && mv_pos.x < 800 &&
		mv_pos.y > 0 - mi_height * 2 && mv_pos.y < 600)
	{
		mp_sprite->Draw(backBuffer);
	}

}

bool
Entity::IsCollidingWith(Entity& e)
{
	//TODO Colision stuff

	return (false);
}

Sprite* 
Entity::GetSprite()
{
	return mp_sprite;
}

int 
Entity::GetWidth()
{
	return mi_width;
}

int 
Entity::GetHeight()
{
	return mi_height;
}

EntityType 
Entity::GetType()
{
	return m_type;
}

void 
Entity::SetWidth(int width)
{
	mi_width = width;
	mp_sprite->SetWidth(static_cast<float>(mi_width));
}

void 
Entity::SetHeight(int height)
{
	mi_height = height;
	mp_sprite->SetHeight(static_cast<float>(mi_height));
}

void
Entity::SetPos(b2Vec2 pos)
{
	mv_pos = pos;
}

b2Vec2&
Entity::GetPos()
{
	return mv_pos;
}
void
Entity::SetDead(bool b)
{
	mb_dead = b;
}

bool
Entity::IsDead()
{
	return mb_dead;
}

