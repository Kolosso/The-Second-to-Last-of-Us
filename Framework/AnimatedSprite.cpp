// This includes:
#include "animatedsprite.h"

// Local includes:
#include "texture.h"
#include "backbuffer.h"

AnimatedSprite::AnimatedSprite()
: mf_frameSpeed()
, mi_frameWidth()
, mf_timeElapsed()
, mi_currentFrame()
, mi_frameCountSprite()
, mi_FPS(0)
, mb_paused()
, mb_loop()
, mb_animating()
{
}

AnimatedSprite::~AnimatedSprite()
{
	//std::vector<int>().swap(frameCoords);
	//Sprite::~Sprite();
}

bool
AnimatedSprite::Initialise(Texture& texture)
{
	mb_loop = false;
	mb_paused = false;
	mb_animating = true;

	Sprite::Initialise(texture);

	int numOfFrames = texture.GetWidth() / mi_frameWidth;
	int frameCoord = 0;
	for (int f = 0; f < numOfFrames; f++)
	{
		frameCoord = f * mi_frameWidth;
		AddFrame(frameCoord);
	}

	StartAnimating();

	return (true);
}

void
AnimatedSprite::AddFrame(int x)
{
	// Add the x coordinate to the frame coordinate container.
	frameCoords.push_back(x);
}

void
AnimatedSprite::Process(float deltaTime)
{
	// TODO If not paused...
	// Count the time elapsed.
	mf_timeElapsed += deltaTime;

	// If the time elapsed is greater than the frame speed.
	// Frame Counter:
	if (mf_timeElapsed >= mf_frameSpeed)
	{
		//m_FPS = m_frameCount;
		//m_frameCount = 0;

		// Move to the next frame.
		mi_currentFrame = frameCoords[mi_frameCountSprite];
		++mi_frameCountSprite;

		// Reset the time elapsed counter.
		mf_timeElapsed = 0;
		// If the current frame is greater than the number 
		//     of frame in this animation...
		// Reset to the first frame.
		if (mi_frameCountSprite >= 5)
		{
			mi_frameCountSprite = 0;
		}
		// TODO Stop the animation if it is not looping...
	}
}

void
AnimatedSprite::Draw(BackBuffer& backBuffer)
{
	// Draw the particular frame into the backbuffer.
	backBuffer.DrawSpriteAnimated(*this, mi_currentFrame, mi_frameWidth);
}

void
AnimatedSprite::SetFrameSpeed(float f)
{
	mf_frameSpeed = f;
}

void
AnimatedSprite::SetFrameWidth(int w)
{
	mi_frameWidth = w;
}


void
AnimatedSprite::Pause()
{
	mb_paused = !mb_paused;
}

bool
AnimatedSprite::IsPaused()
{
	return (mb_paused);
}

bool
AnimatedSprite::IsAnimating()
{
	return (mb_animating);
}

void
AnimatedSprite::StartAnimating()
{
	mb_animating = true;

	mf_timeElapsed = 0;
	mi_currentFrame = 0;
}

bool
AnimatedSprite::IsLooping()
{
	return (mb_loop);
}

void
AnimatedSprite::SetLooping(bool b)
{
	mb_loop = b;
}