// COMP710 GP 2D Framework

// This include:
#include "TextureManager.h"

// Local includes:
#include "Texture.h"
#include "LogManager.h"

// Library includes:
#include <SDL.h>

TextureManager::TextureManager()
: mp_renderer(0)
, mp_texture(nullptr)
{
}

TextureManager::~TextureManager()
{
	std::map<std::string, Texture*>::iterator iter;
	iter = mp_loadedTextures.begin();
	while (iter != mp_loadedTextures.end())
	{
		delete iter->second;
		++iter;
	}
}

bool
TextureManager::Initialise(SDL_Renderer* pRenderer)
{
	mp_renderer = pRenderer;

	return (true);
}

Texture*
TextureManager::GetTexture(const char* pcFilename)
{
	mp_texture = 0;

	if (mp_loadedTextures.find(pcFilename) == mp_loadedTextures.end())
	{
		//Not already loaded, so load
		mp_texture = new Texture();
		if (!mp_texture->Initialise(pcFilename, mp_renderer))
		{
			LogManager::GetInstance().Log("Texture Failed to Init!");
		}
		mp_loadedTextures[pcFilename] = mp_texture;
	}
	else
	{
		//Is already loaded
		mp_texture = mp_loadedTextures[pcFilename];
	}

	return (mp_texture);
}
