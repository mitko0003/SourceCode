#include <stdio.h>
#include <assert.h>
#include <SDL2\SDL.h>
#include "Graphics.h"
#include "Controls.h"
#include "Map.h"
#include "Board.h"
#include "Ghost.h"

std::map <int, std::weak_ptr<SDL_Texture>> loadedTextures;
SDL_Window* screen = nullptr;
SDL_Renderer* renderer = nullptr;

bool initGraphics()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("SDL init error: %s\n", SDL_GetError());
		return false;
	}
	screen = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, 0);
	if (!screen) {
		printf("SDL window open error: %s\n", SDL_GetError());
		return false;
	}
	renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		printf("SDL renderer creation error: %s\n", SDL_GetError());
		return false;
	}
	Board::loadTextures();
	Map::loadTextures();
	Player::loadAnimations();
	Ghost::loadAnimations();
	return true;
}

void clearScreen()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

void render()
{
	board.draw(renderer);
	SDL_RenderPresent(renderer);
}

void quitGraphics()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(screen);
	SDL_Quit();
}

std::shared_ptr<SDL_Texture> loadTexture(const Resource& resource)
{
	auto search = loadedTextures.find(resource.id);
	std::shared_ptr<SDL_Texture> texture = nullptr;
	if (search == loadedTextures.end() || search->second.expired()) {
		texture = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(
			renderer, resource.getAbsolutePath().c_str()), TextureDeleter());
		loadedTextures[resource.id] = texture;
	}
	else {
		texture = search->second.lock();
	}
	if (!texture)
	{
		printf("Can't load texture: %s\n", SDL_GetError());
		exitGame(1);
	}
	return std::move(texture);
}

Texture::Texture(const Resource& resource)
{
	assert(resource.info->type == 'T' && "Bad texture resource info!");
	
	texture = loadTexture(resource);

	const TextureInfo* info = static_cast<const TextureInfo*>(resource.info);
	textureRect = info->textureRect;
}

void Texture::draw(SDL_Renderer* renderer, const SDL_Rect& dest) const
{
	SDL_RenderCopy(renderer, texture.get(), &textureRect, &dest);
}

Animation::Animation(const Resource& resource) : angle(0.0f)
{
	assert(resource.info->type == 'A' && "Bad animation resource info!");

	sprite = loadTexture(resource); 

	const AnimationInfo* info = static_cast<const AnimationInfo*>(resource.info);
	frames = info->frames;
	framesCount = info->framesCount;

	assert(framesCount > 0 && "Invalid frame info!");
	frameDuration = info->duration / framesCount;
}

void Animation::draw(SDL_Renderer* renderer, const SDL_Rect& dest)
{
	int currentTicks = SDL_GetTicks();
	if (lastFrameTicks == 0)
		lastFrameTicks = SDL_GetTicks();
	int skip = currentTicks - lastFrameTicks > frameDuration;
	if (skip) {
		frame = (frame + skip) % framesCount;
		lastFrameTicks = currentTicks;
	}
	SDL_Point pivot = { dest.w / 2, dest.h / 2 };
	SDL_RenderCopyEx(renderer, sprite.get(), frames + frame, &dest, angle, &pivot, SDL_FLIP_NONE);
}

Text::Text(const Resource& resource, const SDL_Color& color, const int& fontSize, const SDL_Rect& position) : fontSize(fontSize), color(color), position(position)
{
	static bool isInit = init();
	assert(resource.info->type == 'F' && "Bad font resource info!");

	font = std::unique_ptr<TTF_Font, FontDeleter>(TTF_OpenFont(resource.getAbsolutePath().c_str(), fontSize), FontDeleter());
	if (!font) {
		printf("TTF open font error: %s", SDL_GetError());
		exitGame(1);
	}
}

void Text::render() 
{
	surface = std::unique_ptr<SDL_Surface, SurfaceDeleter>(TTF_RenderText_Blended(font.get(), text.c_str(), color), SurfaceDeleter());
	if (!surface) {
		printf("TTF render text error: %s", SDL_GetError());
		exitGame(1);
	}
	texture = std::unique_ptr<SDL_Texture, TextureDeleter>(SDL_CreateTextureFromSurface(renderer, surface.get()), TextureDeleter());
	if (!texture) {
		printf("TTF create texture error: %s", SDL_GetError());
		exitGame(1);
	}
	SDL_QueryTexture(texture.get(), NULL, NULL, &position.w, &position.h);
}

bool Text::init()
{
	if (TTF_Init() != 0) {
		printf("SDL ttf init error: %s\n", SDL_GetError());
		exitGame(1);
	}
	return true;
}

void Text::draw(SDL_Renderer * renderer) const
{
	SDL_RenderCopy(renderer, texture.get(), nullptr, &position);
}

void Text::setText(const string & text)
{
	this->text = text;
	render();
	SDL_RenderCopy(renderer, texture.get(), nullptr, &position);
}
