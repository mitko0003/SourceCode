#include "Ghost.h"
#include "Constants.h"
#include "Controls.h"

enum Mode {
	Scatter,
	Chase,
	Frightened,
	Eaten,
};

enum {
	WalkEatenUp,
	WalkEatenDown,
	WalkEatenLeft,
	WalkEatenRight,
	WalkFrightened,
	WalkFrightenedEnd,
	AnimationsCount
};

enum {
	WalkUp,
	WalkDown,
	WalkLeft,
	WalkRight,
	AnimationsCount2
};

Animation Ghost::animations[AnimationsCount];
Animation Pinky::animations[AnimationsCount2];
Animation Inky::animations[AnimationsCount2];
Animation Clyde::animations[AnimationsCount2];
Animation Blinky::animations[AnimationsCount2];
int Ghost::scatterTimer = 0;

Ghost::Ghost(const int& posX, const int& posY) : MobileObject(posX, posY), lastTurnTile(0, 0)
{
	mode = Scatter;
	frightenedTimer = 0;
}

void Ghost::move(Map& map)
{
	bool(*isWalkable)(const char&) = nullptr;
	int movement = 0;
	if (!(movement = frameDistance(getSpeed())))
		return;
	unsigned int frameDuration = SDL_GetTicks() - lastMoveTicks;
	lastMoveTicks += frameDuration;

	int scatterTimer = Ghost::scatterTimer;
	int steps = 1 + ((movement - 1) / (TileSize - 1));
	unsigned int duration = frameDuration / steps;

	for (int step = movement >= TileSize ? TileSize - 1 : movement; movement > 0;
	movement -= step, step = movement >= TileSize ? TileSize - 1 : movement)
	{
		duration += step < TileSize - 1 ? frameDuration % steps : 0;
		if (mode == Frightened) {
			if (frightenedTimer < 0) {
				mode = scatterTimer > 0 ? Scatter : Chase;
				board.player.streak = 1;
			}	
			else
				frightenedTimer -= duration;
		}
		if (scatterTimer < 0) {
			if (mode == Scatter) {
				mode = Chase;
				reverseDirection();
			}
		}
		else
			scatterTimer -= duration;

		if (!caged) {
			switch (mode)
			{
			case Frightened:
				frightenedDirection(map, step);
				isWalkable = isGhostWalkable;
				break;
			case Scatter:
				target = scatter;
				direction(map, step, isScatterWalkable);
				isWalkable = isScatterWalkable;
				break;
			case Chase:
				chase();
				direction(map, step, isGhostWalkable);
				isWalkable = isGhostWalkable;
				break;
			case Eaten:
				target = eatenTarget;
				direction(map, step, isEatenWalkable);
				isWalkable = isEatenWalkable;
				break;
			}
		}
		else {
			target = map.blinkyStart;
			cagedTimer -= duration;
			if (cagedTimer < 0)
				exitDirection(map);
			isWalkable = isCagedWalkable;
			if (reachedTarget()) {
				caged = false;
			}
		}

		int collision;
		switch (getDirection()) {
		case Up:
			posY -= step;
			collision = map.collisionWall(posX, posY, Up, isWalkable);
			posY += collision;
			break;
		case Down:
			posY += step;
			collision = map.collisionWall(posX, posY, Down, isWalkable);
			posY -= collision;
			break;
		case Left:
			posX -= step;
			collision = map.collisionWall(posX, posY, Left, isWalkable);
			posX += collision;
			break;
		case Right:
			posX += step;
			collision = map.collisionWall(posX, posY, Right, isWalkable);
			posX -= collision;
			break;
		}
		if (caged && collision && cagedTimer > 0) 
		{
			reverseDirection();
		}
		if (mode == Eaten && reachedTarget())
		{
			caged = true;
			mode = Chase;
		}
		putOnTrack(map);
		collidePlayer();
	}
	changeAnimation();
}

void Ghost::frightenedDirection(const Map& map, const int& movement)
{
	switch (getDirection())
	{
	case Up:
		if(map.collisionWall(posX, posY - movement, Up, isGhostWalkable))
			setDirection(map.randomTurn(posX, posY, Up));
		break;
	case Down:
		if(map.collisionWall(posX, posY + movement, Down, isGhostWalkable))
			setDirection(map.randomTurn(posX, posY, Down));
		break;
	case Left:
		if(map.collisionWall(posX - movement, posY, Left, isGhostWalkable))
			setDirection(map.randomTurn(posX, posY, Left));
		break;
	case Right:
		if (map.collisionWall(posX + movement, posY, Right, isGhostWalkable))
			setDirection(map.randomTurn(posX, posY, Right));
		break;
	}
}

void Ghost::exitDirection(const Map& map)
{
	if (posX < map.blinkyStart.first)
		setDirection(Right);
	else if (posX >  map.blinkyStart.first)
		setDirection(Left);
	else
		setDirection(Up);
}

void Ghost::direction(const Map & map, const int & movement, bool (*isWalkable)(const char&))
{
	unsigned char newDirection = map.chase(posX, posY, target, getDirection(), movement, isWalkable);
	pos tile = map.findTile(posX, posY);
	if (newDirection != getDirection() && tile != lastTurnTile) {
		lastTurnTile = tile;
		setDirection(newDirection);
	}
}

void Ghost::collidePlayer()
{
	int x = board.player.posX - posX;
	int y = board.player.posY - posY;
	bool collided = x * x + y * y < (TileSize * TileSize);

	if (collided) {
		switch (mode)
		{
		case Frightened:
			mode = Eaten;
			board.player.score += board.player.streak++ * GhostPoints;
			reverseDirection();
			break;
		case Scatter:
		case Chase:
			board.lostLive();
			break;
		}
	}
}

bool Ghost::reachedTarget() const
{
	int x = target.first - posX;
	int y = target.second - posY;
	return x * x + y * y < (TileSize * TileSize);
}

void Ghost::reverseDirection()
{
	switch (getDirection())
	{
	case Up:
		setDirection(Down);
		break;
	case Down:
		setDirection(Up);
		break;
	case Left:
		setDirection(Right);
		break;
	case Right:
		setDirection(Left);
		break;
	}
	lastTurnTile = pos(0, 0);
}

bool Ghost::isCagedWalkable(const char& tile)
{
	return tile == Outside || tile == Door || tile == Path || tile == Dot || tile == Pill;
}

bool Ghost::isScatterWalkable(const char& tile)
{
	return tile == Path || tile == Dot || tile == Pill;
}

bool Ghost::isEatenWalkable(const char & tile)
{
	return tile == Path || tile == Dot || tile == Pill || tile == Outside || tile == Door;
}

bool Ghost::isGhostWalkable(const char& tile)
{
	return tile == Path || tile == Dot || tile == Pill;
}

Pinky::Pinky(const int& posX, const int& posY, const Map& map) : Ghost(posX, posY)
{
	currentAnimation = &animations[WalkUp];
	eatenTarget = map.pinkyStart;
	scatter = pos(3 * TileSize + TileSize / 2, 3 * TileSize + TileSize / 2);
	setDirection(Down);
	cagedTimer = PinkyCagedTime;
	caged = true;
}

Inky::Inky(const int& posX, const int& posY, const Map& map) : Ghost(posX, posY)
{
	currentAnimation = &animations[WalkUp];
	eatenTarget = map.inkyStart;
	scatter = pos((map.width - 3) * TileSize - TileSize / 2, (map.height - 3) * TileSize - TileSize / 2);
	cagedTimer = InkyCagedTime;
	caged = true;
}

Blinky::Blinky(const int& posX, const int& posY, const Map& map) : Ghost(posX, posY)
{
	currentAnimation = &animations[WalkUp];
	eatenTarget = map.pinkyStart;
	scatter = pos((map.width - 3) * TileSize - TileSize / 2, 3 * TileSize + TileSize / 2);
	cagedTimer = 0;
	caged = false;
}


Clyde::Clyde(const int& posX, const int& posY, const Map& map) : Ghost(posX, posY)
{
	currentAnimation = &animations[WalkUp];
	eatenTarget = map.clydeStart;
	scatter = pos(3 * TileSize + TileSize / 2, (map.height - 3) * TileSize - TileSize / 2);
	cagedTimer = ClydeCagedTime;
	caged = true;
}

void Ghost::loadAnimations()
{
	animations[WalkEatenUp] = Animation(AEyesUp);
	animations[WalkEatenDown] = Animation(AEyesDown);
	animations[WalkEatenLeft] = Animation(AEyesLeft);
	animations[WalkEatenRight] = Animation(AEyesRight);
	animations[WalkFrightened] = Animation(AFrightened);
	animations[WalkFrightenedEnd] = Animation(AFrightenedEnd);
	Inky::loadAnimations();
	Pinky::loadAnimations();
	Blinky::loadAnimations();
	Clyde::loadAnimations();
}

void Ghost::putOnTrack(const Map& map)
{
	auto tile = map.findTile(posX, posY);
	char type = map.getTileAt(tile.second, tile.first);
	switch (type) {
	case Path:
	case Dot:
	case Tunel:
	case Pill:
		MobileObject::putOnTrack(map);
		break;
	case Door:
	case Outside:
		switch (getDirection())
		{
		case Up:
		case Down:
			if((int(posX) - int(tile.first * TileSize)) - int(TileSize / 2) < 0)
				posX = tile.first * TileSize;
			else
				posX = (tile.first + 1) * TileSize;
			break;
		case Left:
		case Right:
			posY = tile.second * TileSize + TileSize / 2;
			break;
		}
		break;
	}
}

void Ghost::frighten()
{
	if (mode == Eaten)
		return;
	mode = Frightened;
	reverseDirection();
	frightenedTimer = FrightenTime;
}

void Inky::loadAnimations()
{
	animations[WalkUp] = Animation(AGhostGreenWalkUp);
	animations[WalkDown] = Animation(AGhostGreenWalkDown);
	animations[WalkLeft] = Animation(AGhostGreenWalkLeft);
	animations[WalkRight] = Animation(AGhostGreenWalkRight);
}

void Inky::reset(const Map& map)
{
	caged = true;
	posX = map.inkyStart.first;
	posY = map.inkyStart.second;
	frightenedTimer = 0;
	mode = Scatter;
	cagedTimer = InkyCagedTime;
	MobileObject::reset(map);
}

void Inky::changeAnimation()
{
	switch (mode)
	{
	case Frightened:
		if (frightenedTimer > FrightenTime / 4)
			currentAnimation = Ghost::animations + WalkFrightened;
		else
			currentAnimation = Ghost::animations + WalkFrightenedEnd;
		break;
	case Scatter:
	case Chase:
		currentAnimation = animations + getDirection();
		break;
	case Eaten:
		currentAnimation = Ghost::animations + getDirection();
		break;
	}
}

void Inky::chase()
{
	pos pivot;
	switch (board.player.getDirection())
	{
	case Up:
		pivot = pos(board.player.posX, board.player.posY - 2 * TileSize);
		break;
	case Down:
		pivot = pos(board.player.posX, board.player.posY + 2 * TileSize);
		break;
	case Left:
		pivot = pos(board.player.posX - 2 * TileSize, board.player.posY);
		break;
	case Right:
		pivot = pos(board.player.posX + 2 * TileSize, board.player.posY);
		break;
	}
	target.first = board.blinky.posX + (pivot.first - board.blinky.posX) * 2;
	target.second = board.blinky.posY + (pivot.second - board.blinky.posY) * 2;
}

int Inky::getSpeed() const
{
	return GhostMoveSpeed;
}

void Blinky::loadAnimations()
{
	animations[WalkUp] = Animation(AGhostRedWalkUp);
	animations[WalkDown] = Animation(AGhostRedWalkDown);
	animations[WalkLeft] = Animation(AGhostRedWalkLeft);
	animations[WalkRight] = Animation(AGhostRedWalkRight);
}

void Blinky::reset(const Map& map)
{
	caged = false;
	posX = map.blinkyStart.first;
	posY = map.blinkyStart.second;
	frightenedTimer = 0;
	mode = Scatter;
	cagedTimer = 0;
	MobileObject::reset(map);
}

void Blinky::changeAnimation()
{
	switch (mode)
	{
	case Frightened:
		if (frightenedTimer > FrightenTime / 4)
			currentAnimation = Ghost::animations + WalkFrightened;
		else
			currentAnimation = Ghost::animations + WalkFrightenedEnd;
		break;
	case Scatter:
	case Chase:
		currentAnimation = animations + getDirection();
		break;
	case Eaten:
		currentAnimation = Ghost::animations + getDirection();
		break;
	}
}

void Blinky::chase()
{
	target = pos(board.player.posX, board.player.posY);
}

int Blinky::getSpeed() const
{
	return GhostMoveSpeed;
}

void Clyde::loadAnimations()
{
	animations[WalkUp] = Animation(AGhostYellowWalkUp);
	animations[WalkDown] = Animation(AGhostYellowWalkDown);
	animations[WalkLeft] = Animation(AGhostYellowWalkLeft);
	animations[WalkRight] = Animation(AGhostYellowWalkRight);
}

void Clyde::reset(const Map& map)
{
	caged = true;
	posX = map.clydeStart.first;
	posY = map.clydeStart.second;
	frightenedTimer = 0;
	mode = Scatter;
	cagedTimer = ClydeCagedTime;
	MobileObject::reset(map);
}

void Clyde::changeAnimation()
{
	switch (mode)
	{
	case Frightened:
		if (frightenedTimer > FrightenTime / 4)
			currentAnimation = Ghost::animations + WalkFrightened;
		else
			currentAnimation = Ghost::animations + WalkFrightenedEnd;
		break;
	case Scatter:
	case Chase:
		currentAnimation = animations + getDirection();
		break;
	case Eaten:
		currentAnimation = Ghost::animations + getDirection();
		break;
	}
}

void Clyde::chase()
{
	int x = board.player.posX - board.clyde.posX;
	int y = board.player.posY - board.clyde.posY;
	if (x * x + y * y > TileSize * 8 * TileSize * 8)
		target = pos(board.player.posX, board.player.posY);
	else
		target = scatter;
}

int Clyde::getSpeed() const
{
	return GhostMoveSpeed;
}

void Pinky::loadAnimations()
{
	animations[WalkUp] = Animation(AGhostPinkWalkUp);
	animations[WalkDown] = Animation(AGhostPinkWalkDown);
	animations[WalkLeft] = Animation(AGhostPinkWalkLeft);
	animations[WalkRight] = Animation(AGhostPinkWalkRight);
}

void Pinky::reset(const Map& map)
{
	caged = true;
	posX = map.pinkyStart.first;
	posY = map.pinkyStart.second;
	frightenedTimer = 0;
	mode = Scatter;
	cagedTimer = PinkyCagedTime;
	MobileObject::reset(map);
}

void Pinky::changeAnimation()
{
	switch (mode)
	{
	case Frightened:
		if (frightenedTimer > FrightenTime / 4)
			currentAnimation = Ghost::animations + WalkFrightened;
		else
			currentAnimation = Ghost::animations + WalkFrightenedEnd;
		break;
	case Scatter:
	case Chase:
		currentAnimation = animations + getDirection();
		break;
	case Eaten:
		currentAnimation = Ghost::animations + getDirection();
		break;
	}
}

void Pinky::chase()
{
	switch (board.player.getDirection())
	{
	case Up:
		target = pos(board.player.posX, board.player.posY - 4 * TileSize);
		break;
	case Down:
		target = pos(board.player.posX, board.player.posY + 4 * TileSize);
		break;
	case Left:
		target = pos(board.player.posX - 4 * TileSize, board.player.posY);
		break;
	case Right:
		target = pos(board.player.posX + 4 * TileSize, board.player.posY);
		break;
	}
}

int Pinky::getSpeed() const
{
	return GhostMoveSpeed;
}

