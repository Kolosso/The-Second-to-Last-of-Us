// COMP710 GP 2D Framework
#ifndef __SPRITE_H__
#define __SPRITE_H__
#include <Box2D/Common/b2Math.h>

// Forward declarations:
class BackBuffer;
class Texture;

class Sprite
{
	// Member methods:
public:
	Sprite();
	~Sprite();

	bool Initialise(Texture& texture);

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void SetX(float x);
	void SetY(float y);

	float GetX() const;
	float GetY() const;

	void SetAngle(float angle);
	void SetWidth(float width);
	void SetHeight(float height);
	float GetAngle() const;

	//void SetCenter(int x, int y);
	//void SetHandleCenter();

	int GetWidth() const;
	int GetHeight() const;

	Texture* GetTexture();

	void SetPos(b2Vec2 pos);
	b2Vec2 GetPos();

protected:

private:
	Sprite(const Sprite& sprite);
	Sprite& operator=(const Sprite& sprite);

	// Member data
public:

protected:
	Texture* mp_texture;
	float mf_x;
	float mf_y;

	float mf_angle;
	int mi_centerX;
	int mi_centerY;

	int mi_width;
	int mi_height;

	b2Vec2 mv_pos;

private:

};

#endif // __SPRITE_H__