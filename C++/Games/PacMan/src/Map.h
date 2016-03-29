#pragma once

#include <string>
#include <memory>
#include <SDL2\SDL.h>
#include "Graphics.h"
#include "Constants.h"

/*
	Map format:
	Type: "PM" - 2 bytes
*/

class Map 
{
public:
	pos playerStart;
	pos inkyStart;
	pos pinkyStart;
	pos blinkyStart;
	pos clydeStart;

	Map(const Resource&);
	void draw(SDL_Renderer*, const pos&);
	int collisionWall(const int&, const int&, const unsigned char&, bool (*)(const char&)) const;
	pos findTile(const int&, const int&) const;
	char collectableCheck(const int&, const int&);
	bool isPacmanWalkable(const int&, const int&, bool (*)(const char&)) const;
	const unsigned char randomTurn(const int&, const int&, const unsigned char&) const;
	const unsigned char chase(const int&, const int&, const pos&, const unsigned char&, const int&, bool(*)(const char&)) const;

	inline const char& getTileAt(const int&, const int&) const;
	static void loadTextures();
	int width, height;
	bool isCleared;
private:
	std::string map;
	int lastPillBlink = 0;
	bool shouldDrawPill = true;

	pos startLoacation(const char&, const char&);
	void drawDot(SDL_Renderer*, const SDL_Rect&) const;
	void drawPill(SDL_Renderer*, SDL_Rect);
	void drawInnerWall(SDL_Renderer*, const SDL_Rect&, const int&, const int&) const;
	void drawOuterWall(SDL_Renderer*, const SDL_Rect&, const int&, const int&) const;
	void drawCage(SDL_Renderer*, const SDL_Rect&, const int&, const int&) const;
	void availableTurns(const int&, const int&, unsigned char*, int&, const unsigned char&, bool(*)(const char&)) const;
	bool load(const char*);
	inline void setTileAt(const int&, const int&, const char&);

	static Texture pill;
	static Texture mapTextures[];
};