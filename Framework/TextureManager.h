// COMP710 GP 2D Framework
#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__

// Library Includes:
#include <string>
#include <map>

// Forward Declarations:
class Texture;
struct SDL_Renderer;

class TextureManager
{
	//Member Methods
public:
	TextureManager();
	~TextureManager();

	bool Initialise(SDL_Renderer* pRenderer);

	Texture* GetTexture(const char* pcFilename);

protected:

private:
	TextureManager(const TextureManager& textureManager);
	TextureManager& operator=(const TextureManager& textureManager);

	//Member Data:
public:

protected:
	SDL_Renderer* mp_renderer;
	Texture* mp_texture;
	std::map<std::string, Texture*> mp_loadedTextures;

private:
};

#endif // __TEXTUREMANAGER_H__
