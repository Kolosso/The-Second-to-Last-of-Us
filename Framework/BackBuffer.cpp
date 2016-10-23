// COMP710 GP 2D Framework

// This include:
#include "BackBuffer.h"

// Local includes:
#include "LogManager.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Texture.h"

// Library includes:
#include <SDL.h>
#include <SDL_image.h>
#include <cassert>

BackBuffer::BackBuffer()
: mp_textureManager(0)
, mp_window(0)
, mp_renderer(0)
, mi_height(0)
, mi_width(0)
, muc_clearRed(0xFF)
, muc_clearGreen(0xFF)
, muc_clearBlue(0xFF)
{
}

BackBuffer::~BackBuffer()
{
	SDL_DestroyRenderer(mp_renderer);
	mp_renderer = 0;

	SDL_DestroyWindow(mp_window);
	mp_window = 0;

	delete mp_textureManager;
	mp_textureManager = 0;

	IMG_Quit();
	SDL_Quit();
}

bool
BackBuffer::Initialise(int width, int height)
{
	mi_width = width;
	mi_height = height;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) /*SDL_INIT_JOYSTICK*/
	{
		LogSDLError();
		return (false);
	}
	else
	{
		mp_window = SDL_CreateWindow("The Second to Last of Us", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

		if (mp_window == 0)
		{
			LogSDLError();
			return (false);
		}
		else
		{
			mp_renderer = SDL_CreateRenderer(mp_window, -1, SDL_RENDERER_ACCELERATED);

			if (mp_renderer == 0)
			{
				LogSDLError();
				return (false);
			}
			else
			{
				int imageResult = IMG_Init(IMG_INIT_PNG);

				if (imageResult == 0)
				{
					LogSDLError();
					return (false);
				}
			}
		}
	}

	mp_textureManager = new TextureManager();
	assert(mp_textureManager);
	mp_textureManager->Initialise(mp_renderer);

	return (true);
}

void
BackBuffer::Clear()
{
	SDL_SetRenderDrawColor(mp_renderer, muc_clearRed, muc_clearGreen, muc_clearBlue, 0xff);
	SDL_RenderClear(mp_renderer);
}

void
BackBuffer::Present()
{
	SDL_RenderPresent(mp_renderer);
}

void
BackBuffer::SetDrawColour(unsigned char r, unsigned char g, unsigned char b)
{
	SetDrawColour(r, g, b, 0xFF);
}

void
BackBuffer::SetDrawColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SDL_SetRenderDrawColor(mp_renderer, r, g, b, a);
}

void
BackBuffer::DrawSprite(Sprite& sprite)
{
	SDL_Rect dest;

	dest.x = static_cast<int>(sprite.GetPos().x);
	dest.y = static_cast<int>(sprite.GetPos().y);
	dest.w = sprite.GetWidth();
	dest.h = sprite.GetHeight();
	

	//SDL_RenderCopy(mp_renderer, sprite.GetTexture()->GetTexture(), 0, &dest);
	SDL_RenderCopyEx(mp_renderer, sprite.GetTexture()->GetTexture(), 0, &dest, sprite.GetAngle(), 0, SDL_FLIP_NONE);

}

void
BackBuffer::DrawSpriteAnimated(AnimatedSprite& sprite, int currentFrameX, int frameWidth)
{
	SDL_Rect srce;
	SDL_Rect dest;

	srce.x = currentFrameX;
	srce.y = 0;
	srce.w = frameWidth;
	srce.h = 32;

	dest.x = static_cast<int>(sprite.GetPos().x);
	dest.y = static_cast<int>(sprite.GetPos().y);
	dest.w = frameWidth;
	dest.h = sprite.GetHeight();

	SDL_RenderCopy(mp_renderer, sprite.GetTexture()->GetTexture(), &srce, &dest);
}

void
BackBuffer::DrawRectangle(int x1, int y1, int x2, int y2)
{
	SDL_Rect fillRect;

	fillRect.x = x1;
	fillRect.y = y1;
	fillRect.w = x2 - x1;
	fillRect.h = y2 - y1;

	SDL_RenderFillRect(mp_renderer, &fillRect);
}

void
BackBuffer::DrawLine(int x1, int y1, int x2, int y2)
{
	SDL_RenderDrawLine(mp_renderer, x1, y1, x2, y2);
}

void
BackBuffer::LogSDLError()
{
	LogManager::GetInstance().Log(SDL_GetError());
}

Sprite*
BackBuffer::CreateSprite(const char* pcFilename)
{
	assert(mp_textureManager);

	Texture* pTexture = mp_textureManager->GetTexture(pcFilename);

	Sprite* pSprite = new Sprite();
	if (!pSprite->Initialise(*pTexture))
	{
		LogManager::GetInstance().Log("Sprite Failed to Create!");
	}

	return (pSprite);
}

AnimatedSprite*
BackBuffer::CreateSpriteAnimated(const char* pcFilename)
{
	assert(mp_textureManager);

	Texture* pTexture = mp_textureManager->GetTexture(pcFilename);

	AnimatedSprite* pSprite = new AnimatedSprite();
	if (!pSprite->Initialise(*pTexture))
	{
		LogManager::GetInstance().Log("Sprite Failed to Create!");
	}

	return (pSprite);
}

void
BackBuffer::SetClearColour(unsigned char r, unsigned char g, unsigned char b)
{
	muc_clearRed = r;
	muc_clearGreen = g;
	muc_clearBlue = b;
}
