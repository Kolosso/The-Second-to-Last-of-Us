// COMP710 GP 2D Framework
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

// Library includes:
#include <SDL.h>

class Texture
{
	//Member Methods:
public:
	Texture();
	~Texture();

	bool Initialise(const char* pcFilename, SDL_Renderer* pRenderer);

	int GetWidth() const;
	int GetHeight() const;

	SDL_Texture* GetTexture();

protected:

private:
	Texture(const Texture& texture);
	Texture& operator=(const Texture& texture);

	//Member Data:
public:

protected:
	SDL_Renderer* mp_Renderer;
	SDL_Texture* mp_Texture;

	int mi_width;
	int mi_height;

	SDL_RendererFlip m_flip;

private:
};

#endif // __TEXTURE_H__
