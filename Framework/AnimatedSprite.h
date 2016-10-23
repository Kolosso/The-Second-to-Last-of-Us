#ifndef __ANIMATEDSPRITE_H__
#define __ANIMATEDSPRITE_H__

#include <vector>

#include "sprite.h"

class BackBuffer;
class Texture;

class AnimatedSprite : public Sprite
{
	// Member methods:
public:
	AnimatedSprite();
	~AnimatedSprite();

	bool Initialise(Texture& texture);
	void AddFrame(int x);
	
	void Process(float deltaTime);
	void Draw(BackBuffer& backbuffer);

	void SetFrameSpeed(float f);
	void SetFrameWidth(int w);

	void Pause();
	bool IsPaused();

	void StartAnimating();
	bool IsAnimating();
	
	void SetLooping(bool b);
	bool IsLooping();

protected:

private:

	// Memeber data:
public:

protected:
	std::vector<int> frameCoords;

	float mf_frameSpeed;
	int mi_frameWidth;
	
	float mf_timeElapsed;
	int mi_currentFrame;

	int mi_frameCountSprite;
	int mi_FPS;

	bool mb_paused;
	bool mb_loop;
	bool mb_animating;

private:

};

#endif // __ANIMATEDSPRITE_H__