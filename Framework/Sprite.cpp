// COMP710 GP 2D Framework

// This includes:
#include "sprite.h"

// Local includes:
#include "backbuffer.h"
#include "texture.h"

Sprite::Sprite()
: mp_texture(0)
, mf_x(0)
, mf_y(0)
, mf_angle(0)
, mi_centerX(0)
, mi_centerY(0)
, mi_width(0)
, mi_height(0)
{

}

Sprite::~Sprite()
{

}

bool
Sprite::Initialise(Texture& texture)
{
	mp_texture = &texture;

	mi_width = mp_texture->GetWidth();
	mi_height = mp_texture->GetHeight();

	return (true);
}

void
Sprite::Process(float deltaTIme)
{

}

void
Sprite::Draw(BackBuffer& backBuffer)
{
	backBuffer.DrawSprite(*this);
}

// Getters & Setters
void
Sprite::SetX(float x)
{
	mf_x = x;
}

void
Sprite::SetY(float y)
{
	mf_y = y;
}

float
Sprite::GetX() const
{
	return (mf_x);
}

float
Sprite::GetY() const
{
	return (mf_y);
}

void
Sprite::SetAngle(float angle)
{
	mf_angle = angle;
}

void
Sprite::SetWidth(float width)
{
	mi_width = static_cast<int>(width);
}

void
Sprite::SetHeight(float height)
{
	mi_height = static_cast<int>(height);
}

float
Sprite::GetAngle() const
{
	return (mf_angle);
}

//void
//Sprite::SetCenter(int x, int y)
//{
//	mi_centerX = x;
//	my_centerY = y;
//}
//
//void
//Sprite::SetHandlerCenter()
//{
//	mi_centerX = 50;
//	mi_centerY = 50;
//}

Texture*
Sprite::GetTexture()
{
	return (mp_texture);
}

void 
Sprite::SetPos(b2Vec2 pos)
{
	mv_pos = pos;
}

b2Vec2 
Sprite::GetPos()
{
	return mv_pos;
}

int
Sprite::GetWidth() const
{
	return (mi_width);
}

int
Sprite::GetHeight() const
{
	return (mi_height);
}