// COMP710 GP 2D Framework

// This include:
#include "Texture.h"

// Local includes:
#include "LogManager.h"

// Library include:
#include <SDL_image.h>
#include <cassert>

Texture::Texture()
: mp_Renderer(0)
, mp_Texture(0)
, mi_width(0)
, mi_height(0)
{
	m_flip = SDL_FLIP_NONE;
}

Texture::~Texture()
{
	SDL_DestroyTexture(mp_Texture);
	mp_Texture = 0;
}

bool
Texture::Initialise(const char* pcFilename, SDL_Renderer* pRenderer)
{
	mp_Renderer = pRenderer;

	SDL_Surface* pSurface = IMG_Load(pcFilename);

	if (pSurface == 0)
	{
		LogManager::GetInstance().Log("Image File Failed to Load!");
		return false;
	}
	else
	{
		mp_Texture = SDL_CreateTextureFromSurface(mp_Renderer, pSurface);

		if (mp_Texture == 0)
		{
			LogManager::GetInstance().Log("Texture Failed to Create!");
			return false;
		}

		SDL_FreeSurface(pSurface);
	}

	SDL_SetTextureBlendMode(mp_Texture, SDL_BLENDMODE_BLEND);

	SDL_QueryTexture(mp_Texture, 0, 0, &mi_width, &mi_height);

	return (mp_Texture != 0);
}

int
Texture::GetWidth() const
{
	assert(mi_width);
	return (mi_width);
}

int
Texture::GetHeight() const
{
	assert(mi_height);
	return (mi_height);
}

SDL_Texture*
Texture::GetTexture()
{
	return (mp_Texture);
}
