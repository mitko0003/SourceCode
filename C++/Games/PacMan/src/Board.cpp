#include <string>
#include "Board.h"
#include "Constants.h"
#include "Controls.h"

Texture Board::live;
Texture Board::cherry;
Animation Player::walk;

void Board::loadTextures()
{
	cherry = Texture(TCherry);
	live = Texture(TLive);
}

void Board::initTimers()
{
	Ghost::scatterTimer = ScatterTime;
	unsigned int currentTicks = SDL_GetTicks();
	player.lastMoveTicks = currentTicks;
	blinky.lastMoveTicks = currentTicks;
	clyde.lastMoveTicks = currentTicks;
	inky.lastMoveTicks = currentTicks;
}

void Board::draw(SDL_Renderer* renderer)
{
	map.draw(renderer, drawLocation);
	player.draw(renderer, drawLocation);
	clyde.draw(renderer, drawLocation);
	inky.draw(renderer, drawLocation);
	pinky.draw(renderer, drawLocation);
	blinky.draw(renderer, drawLocation);
	drawIngameInfo(renderer);
}

void Board::onPillCollected()
{
	inky.frighten();
	blinky.frighten();
	clyde.frighten();
	pinky.frighten();
}

void Board::update()
{
	static unsigned int lastUpdateTicks = SDL_GetTicks();
	if (map.isCleared)
		exitGame(0);

	unsigned int currentTicks = SDL_GetTicks();

	player.move(map);
	blinky.move(map);
	pinky.move(map);
	inky.move(map);
	clyde.move(map);

	currentTicks = (currentTicks + SDL_GetTicks()) / 2;
	if (Ghost::scatterTimer > 0)
		Ghost::scatterTimer -= currentTicks - lastUpdateTicks;
	lastUpdateTicks = currentTicks;
}

Board::Board() : 
	map(MMap), 
	player(map.playerStart.first, map.playerStart.second), 
	inky(map.inkyStart.first, map.inkyStart.second, map),
	pinky(map.pinkyStart.first, map.pinkyStart.second, map),
	clyde(map.clydeStart.first, map.clydeStart.second, map),
	blinky(map.blinkyStart.first, map.blinkyStart.second, map),
	drawLocation(WindowWidth / 2 - map.width * TileSize / 2, WindowHeight / 2 - map.height * TileSize / 2),
	score(FDefaultFont, IngameInfoTextColor, IngameInfoTextSize, { drawLocation.first + int(TileSize), drawLocation.second - int(TileSize), 0, 0 }) {}

void Board::drawScore(SDL_Renderer* renderer)
{
	score.setText("Score: " + std::to_string(player.score));
	score.draw(renderer);
}

void Board::lostLive()
{
	if (!(--player.lives)) 
		exitGame(0);

	initTimers();
	inky.reset(map);
	pinky.reset(map);
	clyde.reset(map);
	blinky.reset(map);
	player.reset(map);
}

void Board::drawIngameInfo(SDL_Renderer * renderer)
{
	SDL_Rect tile;
	tile.w = tile.h = TileSize;
	tile.x = TileSize + drawLocation.first;
	tile.y = map.height * TileSize + drawLocation.second;

	for (int i = 0; i < player.lives; ++i, tile.x += (TileSize * 3) / 2)
	{
		live.draw(renderer, tile);
	}
	drawScore(renderer);
}

Player::Player(const int& posX, const int& posY) : MobileObject(posX, posY) 
{
	currentAnimation = &walk;
}

void Player::tryTurn(const Map& map)
{
	auto tile = map.findTile(posX, posY);
	switch (getTurnDirection())
	{
	case Up:
		if (map.isPacmanWalkable(tile.second - 1, tile.first, isPlayerWalkable)) {
			setDirection(getTurnDirection());
			currentAnimation->angle = -90.0f;
		}
		break;
	case Down:
		if (map.isPacmanWalkable(tile.second + 1, tile.first, isPlayerWalkable)) {
			setDirection(getTurnDirection());
			currentAnimation->angle = 90.0f;
		}
		break;
	case Left:
		if (map.isPacmanWalkable(tile.second, tile.first - 1, isPlayerWalkable)) {
			setDirection(getTurnDirection());
			currentAnimation->angle = 180.0f;
		}
		break;
	case Right:
		if (map.isPacmanWalkable(tile.second, tile.first + 1, isPlayerWalkable)) {
			setDirection(getTurnDirection());
			currentAnimation->angle = 0.0f;
		}
		break;
	}
}

void Player::move(Map& map)
{
	int movement = 0;
	if (!(movement = frameDistance(PacmanMoveSpeed)))
		return;
	lastMoveTicks = SDL_GetTicks();
	
	for (int step = movement >= TileSize ? TileSize - 1 : movement; movement > 0; 
			movement -= step, step = movement >= TileSize ? TileSize - 1 : movement) {
		switch (getDirection()) {
		case Up:
			posY -= step;
			posY += map.collisionWall(posX, posY, Up, isPlayerWalkable);
			break;
		case Down:
			posY += step;
			posY -= map.collisionWall(posX, posY, Down, isPlayerWalkable);
			break;
		case Left:
			posX -= step;
			posX += map.collisionWall(posX, posY, Left, isPlayerWalkable);
			break;
		case Right:
			posX += step;
			posX -= map.collisionWall(posX, posY, Right, isPlayerWalkable);
			break;
		}

		switch (map.collectableCheck(posX, posY))
		{
		case Dot:
			score += DotPoints;
			++dotsEaten;
			break;
		case Pill:
			score += PillPoints;
			board.onPillCollected();
			break;
		}

		tryTurn(map);
	}
	putOnTrack(map);
}

void Player::turn(const unsigned char& direction)
{
	setTurnDirection(direction);
}

void Player::reset(const Map& map)
{
	posX = map.playerStart.first;
	posY = map.playerStart.second;
	walk.angle = 0;
	MobileObject::reset(map);
}

void Player::loadAnimations()
{
	walk = Animation(APacmanWalk);
}

bool Player::isPlayerWalkable(const char& tile)
{
	return tile == Path || tile == Dot || tile == Pill || tile == Tunel;
}
