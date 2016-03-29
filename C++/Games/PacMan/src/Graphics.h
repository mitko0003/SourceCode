#pragma once

#include <map>
#include <memory>
#include <string>
#include <SDL2\SDL_image.h>
#include <SDL2\SDL_ttf.h>
#include "Constants.h"

extern std::map <int, std::weak_ptr<SDL_Texture>> loadedTextures;
extern SDL_Window* window;
extern SDL_Renderer* renderer;

bool initGraphics();
void render();
void clearScreen();
void quitGraphics(); 
std::shared_ptr<SDL_Texture> loadTexture(const Resource&);
SDL_Texture* renderText(const char*, const char*, const SDL_Color&, const int&, SDL_Renderer*);

class TextureDeleter
{
public:
	void operator()(SDL_Texture* texture)
	{
		SDL_DestroyTexture(texture);
	}
};

class FontDeleter
{
public:
	void operator()(TTF_Font* font)
	{
		TTF_CloseFont(font);
	}
};

class SurfaceDeleter
{
public:
	void operator()(SDL_Surface* surface)
	{
		SDL_FreeSurface(surface);
	}
};

class Text
{
public:
	Text(const Resource&, const SDL_Color&, const int&, const SDL_Rect&);
	void draw(SDL_Renderer*) const;
	void setText(const string&);
private:
	string text;
	std::unique_ptr<TTF_Font, FontDeleter> font;
	std::unique_ptr<SDL_Surface, SurfaceDeleter> surface;
	std::unique_ptr<SDL_Texture, TextureDeleter> texture;
	SDL_Rect position;
	SDL_Color color;
	unsigned int fontSize;
	void render();
	static bool init();
};

class Texture
{
public:
	Texture(const Resource&);
	Texture() : texture(nullptr) {}
	void draw(SDL_Renderer*, const SDL_Rect&) const;
private:
	std::shared_ptr<SDL_Texture> texture = nullptr;
	SDL_Rect textureRect;
};

class Animation
{
public:
	Animation(const Resource&);
	Animation() : sprite(nullptr), frames(nullptr) {}
	void draw(SDL_Renderer*, const SDL_Rect&);
	double angle;
private:
	std::shared_ptr<SDL_Texture> sprite = nullptr;
	const SDL_Rect* frames;
	unsigned int frame = 0;
	unsigned int lastFrameTicks = 0;
	unsigned int frameDuration;
	unsigned int framesCount;
};