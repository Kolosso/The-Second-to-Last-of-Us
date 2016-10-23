// COMP710 GP 2D Framework
#ifndef __BACKBUFFER_H__
#define __BACKBUFFER_H__

//Forward Declarations
class TextureManager;
class Sprite;
class AnimatedSprite;
struct SDL_Window;
struct SDL_Renderer;

class BackBuffer
{
	//Member Methods:
public:
	BackBuffer();
	~BackBuffer();

	bool Initialise(int width, int height);

	void Clear();
	void Present();

	void SetClearColour(unsigned char r, unsigned char g, unsigned char b);
	void SetDrawColour(unsigned char r, unsigned char g, unsigned char b);
	void SetDrawColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	void DrawSprite(Sprite& sprite);
	void DrawSpriteAnimated(AnimatedSprite& sprite, int currentFrameX, int frameWidth);
	void DrawRectangle(int x1, int y1, int x2, int y2);
	void DrawLine(int x1, int y1, int x2, int y2);

	Sprite* CreateSprite(const char* pcFilename);
	AnimatedSprite* CreateSpriteAnimated(const char* pcFilename);

protected:
	void LogSDLError();

private:
	BackBuffer(const BackBuffer& backBuffer);
	BackBuffer& operator=(const BackBuffer& backBuffer);

	//Member Data:
public:

protected:
	TextureManager* mp_textureManager;

	SDL_Window* mp_window;
	SDL_Renderer* mp_renderer;

	int mi_height;
	int mi_width;

	unsigned char muc_clearRed;
	unsigned char muc_clearGreen;
	unsigned char muc_clearBlue;
};

#endif // __BACKBUFFER_H__
