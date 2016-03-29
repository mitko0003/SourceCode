#include <SDL2\SDL.h>
#include <SDL2\SDL_events.h>
#include <cstdlib>
#include "Controls.h"
#include "Graphics.h"

bool isRunning = true;
bool isInGame = true;
Board board;

void handleInput()
{
	SDL_Event curEvent;
	while (SDL_PollEvent(&curEvent)) {
		switch (curEvent.type) 
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			switch (curEvent.key.keysym.sym) {
			case SDLK_w:
			case SDLK_UP:
				board.player.turn(Up);
				break;
			case SDLK_s:
			case SDLK_DOWN:
				board.player.turn(Down);
				break;
			case SDLK_a:
			case SDLK_LEFT:
				board.player.turn(Left);
				break;
			case SDLK_d:
			case SDLK_RIGHT:
				board.player.turn(Right);
				break;
			}
			break;
		}
	}
}

void exitGame(const int& status)
{
	SDL_Delay(3000);
	quitGraphics();
	std::exit(status);
}

Fps::Fps(const int & fps) : fps(fps)
{
	frameDuration = 1000 / fps;
	frameCounter = 0;
	lastSecondFPS = 0;
	lastRefresh = SDL_GetTicks();
	lastSecondTicks = lastRefresh;
}

void Fps::limit()
{
	++frameCounter;
	int currentTicks = SDL_GetTicks();
	int lastFrameTicks = currentTicks - lastRefresh;
	if (lastFrameTicks < frameDuration){
		SDL_Delay(frameDuration - lastFrameTicks);
		lastRefresh += lastFrameTicks;
	}
	lastRefresh = currentTicks;
	if (currentTicks - lastSecondTicks >= 1000) {
		lastSecondFPS = 1000.0f / frameCounter;
		frameCounter = 0;
		lastSecondTicks = currentTicks;
	}
}

float Fps::measure()
{
	return lastSecondFPS;
}
