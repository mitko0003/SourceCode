#include "MobileObject.h"
#include "Controls.h"

MobileObject::MobileObject(const int& posX, const int& posY) : posX(posX), posY(posY)
{
	movementInfo = 0;
	setDirection(Up);
}

void MobileObject::draw(SDL_Renderer* renderer, const pos& position)
{
	SDL_Rect tile = { posX - TileSize + position.first, posY - TileSize + position.second,  TileSize * 2, TileSize * 2 };
	currentAnimation->draw(renderer, tile);
}

void MobileObject::putOnTrack(const Map& map)
{
	auto tile = map.findTile(posX, posY);
	switch (getDirection())
	{
	case Up:
	case Down:
		posX = tile.first * TileSize + TileSize / 2;
		break;
	case Left:
	case Right:
		posY = tile.second * TileSize + TileSize / 2;
		break;
	}
}

void MobileObject::reset(const Map &)
{
	lastMoveTicks = SDL_GetTicks();
	movementInfo = 0;
}

int MobileObject::frameDistance(const int& speed) const
{
	int currentTicks = SDL_GetTicks();
	int movement = int(speed * ((currentTicks - lastMoveTicks) / 1000.0f));
	if (movement) {
		return movement;
	}
	else
		return 0;
}