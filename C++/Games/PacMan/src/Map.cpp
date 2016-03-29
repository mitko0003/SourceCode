#include <assert.h>
#include <stdio.h>
#include "Map.h"
#include "Board.h"
#include "Controls.h"

const char* MAP =
"oooooooooooooooooooooooooooooo"
"oWWWWWWWWWWWWWWWWWWWWWWWWWWWWo"
"oWPPPPPPPPPPPPwwPPPPPPPPPPPPWo"
"oWPwwwwPwwwwwPwwPwwwwwPwwwwPWo"
"oWHwoowPwooowPwwPwooowPwoowHWo"
"oWPwwwwPwwwwwPwwPwwwwwPwwwwPWo"
"oWPPPPPPPPPPPPPPPPPPPPPPPPPPWo"
"oWPwwwwPwwPwwwwwwwwPwwPwwwwPWo"
"oWPwwwwPwwPwwwwwwwwPwwPwwwwPWo"
"oWPPPPPPwwPPPPwwPPPPwwPPPPPPWo"
"oWWWWWWPwwwwwpwwpwwwwwPWWWWWWo"
"ooooooWPwwwwwpwwpwwwwwPWoooooo"
"ooooooWPwwppppbbppppwwPWoooooo"
"ooooooWPwwpcccddcccpwwPWoooooo"
"oWWWWWWPwwpcoooooocpwwPWWWWWWo"
"ottttttPpppciirrzzcpppPtttttto"
"oWWWWWWPwwpcoooooocpwwPWWWWWWo"
"ooooooWPwwpccccccccpwwPWoooooo"
"ooooooWPwwppppppppppwwPWoooooo"
"ooooooWPwwpwwwwwwwwpwwPWoooooo"
"oWWWWWWPwwpwwwwwwwwpwwPWWWWWWo"
"oWPPPPPPPPPPPPwwPPPPPPPPPPPPWo"
"oWPwwwwPwwwwwPwwPwwwwwPwwwwPWo"
"oWPwwwwPwwwwwPwwPwwwwwPwwwwPWo"
"oWHPPwwPPPPPPPssPPPPPPPwwPPHWo"
"oWwwPwwPwwPwwwwwwwwPwwPwwPwwWo"
"oWwwPwwPwwPwwwwwwwwPwwPwwPwwWo"
"oWPPPPPPwwPPPPwwPPPPwwPPPPPPWo"
"oWPwwwwwwwwwwPwwPwwwwwwwwwwPWo"
"oWPwwwwwwwwwwPwwPwwwwwwwwwwPWo"
"oWPPPPPPPPPPPPPPPPPPPPPPPPPPWo"
"oWWWWWWWWWWWWWWWWWWWWWWWWWWWWo"
"oooooooooooooooooooooooooooooo";

enum {
	InnerTopWall,
	InnerBottomWall,
	InnerLeftWall,
	InnerRightWall,
	InnerTopLeftCorner,
	InnerTopRightCorner,
	InnerBottomLeftCorner,
	InnerBottomRightCorner,
	InnerTopLeftCorner2,
	InnerTopRightCorner2,
	InnerBottomLeftCorner2,
	InnerBottomRightCorner2,
	OuterTopWall,
	OuterBottomWall,
	OuterLeftWall,
	OuterRightWall,
	OuterTopLeftCorner,
	OuterTopRightCorner,
	OuterBottomLeftCorner,
	OuterBottomRightCorner,
	OuterTopLeftCorner2,
	OuterTopRightCorner2,
	OuterBottomLeftCorner2,
	OuterBottomRightCorner2,
	DoorIndex,
	DoorStepLeft, 
	DoorStepRight,
	CageTopLeftCorner,
	CageTopRightCorner,
	CageBottomLeftCorner,
	CageBottomRightCorner,
	InOutTopLeft,
	InOutTopRight,
	InOutBottomLeft,
	InOutBottomRight,
	InOutLeftTop,
	InOutRightTop,
	InOutLeftBottom,
	InOutRightBottom,
	MapTexturesCount
};

Texture Map::pill;
Texture Map::mapTextures[MapTexturesCount];

Map::Map(const Resource& resource)
{
	assert(resource.info->type == 'M' && "Bad map resource info!");
	load(resource.getAbsolutePath().c_str());
	playerStart = startLoacation(PlayerChar, Path);
	inkyStart = startLoacation(InkyChar, Outside);
	blinkyStart = startLoacation(BlinkyChar, Path);
	clydeStart = startLoacation(ClydeChar, Outside);
	pinkyStart = startLoacation(PinkyChar, Outside);
}

void Map::loadTextures()
{
	Map::pill = Texture(TPill);
	Map::mapTextures[InnerTopWall]            = Texture(TWallInnerTop);
	Map::mapTextures[InnerBottomWall]         = Texture(TWallInnerBottom);
	Map::mapTextures[InnerLeftWall]           = Texture(TWallInnerLeft);
	Map::mapTextures[InnerRightWall]          = Texture(TWallInnerRight);
	Map::mapTextures[InnerTopLeftCorner]      = Texture(TCornerInnerTopLeft);
	Map::mapTextures[InnerTopRightCorner]     = Texture(TCornerInnerTopRight);
	Map::mapTextures[InnerBottomLeftCorner]   = Texture(TCornerInnerBottomLeft);
	Map::mapTextures[InnerBottomRightCorner]  = Texture(TCornerInnerBottomRight);
	Map::mapTextures[InnerTopLeftCorner2]     = Texture(TCornerInnerTopLeft2);
	Map::mapTextures[InnerTopRightCorner2]    = Texture(TCornerInnerTopRight2);
	Map::mapTextures[InnerBottomLeftCorner2]  = Texture(TCornerInnerBottomLeft2);
	Map::mapTextures[InnerBottomRightCorner2] = Texture(TCornerInnerBottomRight2);
	Map::mapTextures[OuterTopWall]			  = Texture(TWallOuterTop);
	Map::mapTextures[OuterBottomWall]		  = Texture(TWallOuterBottom);
	Map::mapTextures[OuterLeftWall]           = Texture(TWallOuterLeft);
	Map::mapTextures[OuterRightWall]          = Texture(TWallOuterRight);
	Map::mapTextures[OuterTopLeftCorner]      = Texture(TCornerOuterTopLeft);
	Map::mapTextures[OuterTopRightCorner]     = Texture(TCornerOuterTopRight);
	Map::mapTextures[OuterBottomLeftCorner]   = Texture(TCornerOuterBottomLeft);
	Map::mapTextures[OuterBottomRightCorner]  = Texture(TCornerOuterBottomRight);
	Map::mapTextures[OuterTopLeftCorner2]     = Texture(TCornerOuterTopLeft2);
	Map::mapTextures[OuterTopRightCorner2]    = Texture(TCornerOuterTopRight2);
	Map::mapTextures[OuterBottomLeftCorner2]  = Texture(TCornerOuterBottomLeft2);
	Map::mapTextures[OuterBottomRightCorner2] = Texture(TCornerOuterBottomRight2);
	Map::mapTextures[DoorIndex]               = Texture(TDoor);
	Map::mapTextures[DoorStepLeft]            = Texture(TDoorStepLeft);
	Map::mapTextures[DoorStepRight]           = Texture(TDoorStepRight);
	Map::mapTextures[CageTopLeftCorner]       = Texture(TCornerCageTopLeft);
	Map::mapTextures[CageTopRightCorner]      = Texture(TCornerCageTopRight);
	Map::mapTextures[CageBottomLeftCorner]    = Texture(TCornerCageBottomLeft);
	Map::mapTextures[CageBottomRightCorner]   = Texture(TCornerCageBottomRight);
	Map::mapTextures[InOutTopLeft]            = Texture(TInOutTopLeft);
	Map::mapTextures[InOutTopRight]           = Texture(TInOutTopRight);
	Map::mapTextures[InOutBottomLeft]         = Texture(TInOutBottomLeft);
	Map::mapTextures[InOutBottomRight]        = Texture(TInOutBottomRight);
	Map::mapTextures[InOutLeftTop]            = Texture(TInOutLeftTop);
	Map::mapTextures[InOutRightTop]           = Texture(TInOutRightTop);
	Map::mapTextures[InOutLeftBottom]         = Texture(TInOutLeftBottom);
	Map::mapTextures[InOutRightBottom]        = Texture(TInOutRightBottom);
}

inline const char& Map::getTileAt(const int& col, const int& row) const
{
	assert((row >= 0 || col >= 0 || row < height || col < width) && "Invalid map tile (out of bounds)!");
	return map[col * width + row];
}

bool Map::load(const char* path)
{
	map = MAP;
	width = 30;
	height = 33;
	return true;
	// TODO : load from file
}

inline void Map::setTileAt(const int& y, const int& x, const char & tile)
{
	assert((x >= 0 || y >= 0 || x < width || y < height) && "Invalid map tile (out of bounds)!");
	map[y * width + x] = tile;
}

pos Map::startLoacation(const char& oldChar, const char& newChar)
{
	int start = map.find(oldChar);
	map[start] = newChar;
	map[start + 1] = newChar;

	pos result;
	result.first = (start % width) * TileSize + TileSize;
	result.second = (start / width) * TileSize + TileSize / 2;
	return result;
}

void Map::drawDot(SDL_Renderer* renderer, const SDL_Rect& tile) const
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_SetRenderDrawColor(renderer, DotColor[0], DotColor[1], DotColor[2], DotColor[3]);
	SDL_RenderSetScale(renderer, DotSize, DotSize);
	SDL_RenderDrawPoint(renderer, (tile.x + (tile.w >> 1)) / int(DotSize), (tile.y + (tile.h >> 1)) / int(DotSize));
	SDL_RenderSetScale(renderer, 1.0f, 1.0f);
}

void Map::drawPill(SDL_Renderer* renderer, SDL_Rect tile)
{
	int currentTicks = SDL_GetTicks();
	if (currentTicks - lastPillBlink > PillBlinkTicks) {
		shouldDrawPill = !shouldDrawPill;
		lastPillBlink += PillBlinkTicks;
	}
	if (shouldDrawPill) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		tile.x += (tile.w >> 1) - PillSize;
		tile.y += (tile.h >> 1) - PillSize;
		tile.w = PillSize * 2;
		tile.h = PillSize * 2;
		pill.draw(renderer, tile);
	}
}

void Map::drawInnerWall(SDL_Renderer* renderer, const SDL_Rect& tile, const int& x, const int& y) const
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	const char& top    = getTileAt(x - 1, y);
	const char& bottom = getTileAt(x + 1, y);
	const char& left   = getTileAt(x, y - 1);
	const char& right  = getTileAt(x, y + 1);

	const char& topLeft     = getTileAt(x - 1, y - 1);
	const char& bottomLeft  = getTileAt(x + 1, y - 1);
	const char& topRight    = getTileAt(x - 1, y + 1);
	const char& bottomRight = getTileAt(x + 1, y + 1);

	if ((top == InnerWall || top == OuterWall) && (bottom == InnerWall || bottom == OuterWall) && (left == InnerWall || left == Outside) && right != InnerWall)
		mapTextures[InnerRightWall].draw(renderer, tile);
	else if ((top == InnerWall || top == OuterWall) && (bottom == InnerWall || bottom == OuterWall) && left != InnerWall && (right == InnerWall || right == Outside))
		mapTextures[InnerLeftWall].draw(renderer, tile);
	else if ((top == InnerWall || top == Outside) && bottom != InnerWall && (left == InnerWall || left == OuterWall) && (right == InnerWall || right == OuterWall))
		mapTextures[InnerTopWall].draw(renderer, tile);
	else if (top != InnerWall && (bottom == InnerWall || bottom == Outside) && (left == InnerWall || left == OuterWall) && (right == InnerWall || right == OuterWall))
		mapTextures[InnerBottomWall].draw(renderer, tile);
	
	else if (top == InnerWall && bottom != InnerWall && left == InnerWall && right != InnerWall)
		mapTextures[InnerBottomRightCorner].draw(renderer, tile);
	else if (top == InnerWall && bottom != InnerWall && left != InnerWall && right == InnerWall)
		mapTextures[InnerBottomLeftCorner].draw(renderer, tile);
	else if (top != InnerWall && bottom == InnerWall && left == InnerWall && right != InnerWall)
		mapTextures[InnerTopRightCorner].draw(renderer, tile);
	else if (top != InnerWall && bottom == InnerWall && left != InnerWall && right == InnerWall)
		mapTextures[InnerTopLeftCorner].draw(renderer, tile);

	else if (top == InnerWall && bottom == InnerWall && left == InnerWall && right == InnerWall) {
		if (topLeft != InnerWall)
			mapTextures[InnerBottomRightCorner2].draw(renderer, tile);
		else if (topRight != InnerWall)
			mapTextures[InnerBottomLeftCorner2].draw(renderer, tile);
		else if (bottomLeft != InnerWall)
			mapTextures[InnerTopRightCorner2].draw(renderer, tile);
		else if (bottomRight != InnerWall)
			mapTextures[InnerTopLeftCorner2].draw(renderer, tile);
	}
}

void Map::drawOuterWall(SDL_Renderer* renderer, const SDL_Rect& tile, const int& x, const int& y) const
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	const char& top = getTileAt(x - 1, y);
	const char& bottom = getTileAt(x + 1, y);
	const char& left = getTileAt(x, y - 1);
	const char& right = getTileAt(x, y + 1);

	const char& topLeft = getTileAt(x - 1, y - 1);
	const char& bottomLeft = getTileAt(x + 1, y - 1);
	const char& topRight = getTileAt(x - 1, y + 1);
	const char& bottomRight = getTileAt(x + 1, y + 1);

	if (top == OuterWall && bottom != OuterWall && left == OuterWall && right != OuterWall) {
		if (topLeft == Outside)
			mapTextures[OuterBottomRightCorner].draw(renderer, tile);
		if (bottomRight == Outside)
			mapTextures[OuterBottomRightCorner2].draw(renderer, tile);
	}
	else if (top != OuterWall && bottom == OuterWall && left == OuterWall && right != OuterWall) {
		if (bottomLeft == Outside)
			mapTextures[OuterTopRightCorner].draw(renderer, tile);
		else if (topRight == Outside)
			mapTextures[OuterTopRightCorner2].draw(renderer, tile);
	}
	else if (top != OuterWall && bottom == OuterWall && left != OuterWall && right == OuterWall) {
		if (bottomRight == Outside)
			mapTextures[OuterTopLeftCorner].draw(renderer, tile);
		else if (topLeft == Outside)
			mapTextures[OuterTopLeftCorner2].draw(renderer, tile);
	}
	else if (top == OuterWall && bottom != OuterWall && left != OuterWall && right == OuterWall) {
		if (topRight == Outside)
			mapTextures[OuterBottomLeftCorner].draw(renderer, tile);
		else if (bottomLeft == Outside)
			mapTextures[OuterBottomLeftCorner2].draw(renderer, tile);
	}

	else if ((top == OuterWall || top == Outside) && (bottom == OuterWall || bottom == Outside)) {
		if (left != OuterWall && right == Outside) {
			if (left == InnerWall) {
				if (topLeft == InnerWall)
					mapTextures[InOutRightBottom].draw(renderer, tile);
				else if (bottomLeft == InnerWall)
					mapTextures[InOutRightTop].draw(renderer, tile);
			}
			else
				mapTextures[OuterRightWall].draw(renderer, tile);
		}
		else if (left == Outside && right != OuterWall) {
			if (right == InnerWall) {
				if (topRight == InnerWall)
					mapTextures[InOutLeftBottom].draw(renderer, tile);
				else if (bottomRight == InnerWall)
					mapTextures[InOutLeftTop].draw(renderer, tile);
			}
			else
				mapTextures[OuterLeftWall].draw(renderer, tile);
		}
	}
	else if ((left == OuterWall || left == Outside) && (right == OuterWall || right == Outside)) {
		if (top != OuterWall && bottom == Outside) {
			if (top == InnerWall) {
				if (topLeft == InnerWall)
					mapTextures[InOutBottomRight].draw(renderer, tile);
				else if (topRight == InnerWall)
					mapTextures[InOutBottomLeft].draw(renderer, tile);
			}
			else
				mapTextures[OuterBottomWall].draw(renderer, tile);
		}
		else if (top == Outside && bottom != OuterWall) {
			if (bottom == InnerWall) {
				if (bottomLeft == InnerWall)
					mapTextures[InOutTopRight].draw(renderer, tile);
				else if (bottomRight == InnerWall)
					mapTextures[InOutTopLeft].draw(renderer, tile);
			}
			else
				mapTextures[OuterTopWall].draw(renderer, tile);
		}
	}
}


void Map::drawCage(SDL_Renderer* renderer, const SDL_Rect& tile, const int& x, const int& y) const
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	const char& top = getTileAt(x - 1, y);
	const char& bottom = getTileAt(x + 1, y);
	const char& left = getTileAt(x, y - 1);
	const char& right = getTileAt(x, y + 1);

	if (top == Cage && bottom == Cage && left != Cage && right == Path)
		mapTextures[OuterLeftWall].draw(renderer, tile);
	else if (top == Cage && bottom == Cage && left == Path && right != Cage)
		mapTextures[OuterRightWall].draw(renderer, tile);
	else if (top != Cage && bottom == Path && left == Cage && right == Cage)
		mapTextures[OuterTopWall].draw(renderer, tile);
	else if (top == Path && bottom != Cage && left == Cage && right == Cage)
		mapTextures[OuterBottomWall].draw(renderer, tile);

	else if (top == Cage && bottom != Cage && left == Cage && right != Cage)
		mapTextures[CageBottomRightCorner].draw(renderer, tile);
	else if (top == Cage && bottom != Cage && left != Cage && right == Cage)
		mapTextures[CageBottomLeftCorner].draw(renderer, tile);
	else if (top != Cage && bottom == Cage && left == Cage && right != Cage)
		mapTextures[CageTopRightCorner].draw(renderer, tile);
	else if (top != Cage && bottom == Cage && left != Cage && right == Cage)
		mapTextures[CageTopLeftCorner].draw(renderer, tile);

	else if (left == Door)
		mapTextures[DoorStepRight].draw(renderer, tile);
	else if (right == Door)
		mapTextures[DoorStepLeft].draw(renderer, tile);
}

void Map::availableTurns(const int & x, const int & y, unsigned char* directions, int& count, const unsigned char & direction, bool (*isWalkable)(const char&)) const
{
	pos tile = findTile(x, y);

	if (isPacmanWalkable(tile.second - 1, tile.first, isWalkable) && Up != direction && Down != direction)
		directions[count++] = Up;
	if (isPacmanWalkable(tile.second + 1, tile.first, isWalkable) && Up != direction && Down != direction)
		directions[count++] = Down;
	if (isPacmanWalkable(tile.second, tile.first - 1, isWalkable) && Left != direction && Right != direction)
		directions[count++] = Left;
	if (isPacmanWalkable(tile.second, tile.first + 1, isWalkable) && Left != direction && Right != direction)
		directions[count++] = Right;
}

void Map::draw(SDL_Renderer* renderer, const pos& position)
{
	SDL_Rect tile = { 0, 0, TileSize, TileSize };
	isCleared = true;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile.x = TileSize * j + position.first;
			tile.y = TileSize * i + position.second;
			switch (getTileAt(i, j))
			{
			case Cage:
				drawCage(renderer, tile, i, j);
				break;
			case Pill:
				drawPill(renderer, tile);
				isCleared = false;
				break;
			case Door:
				mapTextures[DoorIndex].draw(renderer, tile);
				break;
			case Path:
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
				break;
			case Dot:
				drawDot(renderer, tile);
				isCleared = false;
				break;
			case Outside:
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				break;
			case InnerWall:
				drawInnerWall(renderer, tile, i, j);
				break;
			case OuterWall:
				drawOuterWall(renderer, tile, i, j);
				break;
			}
		}
	}
}

int Map::collisionWall(const int & posX, const int & posY, const unsigned char& direction, bool(*isWalkable)(const char&)) const
{
	int edge; 
	char tile;
	switch (direction)
	{
	case Up:
		edge = posY - (TileSize >> 1);
		tile = getTileAt(edge / TileSize, posX / TileSize);
		if (!isWalkable(tile))
			return (1 + (edge - 1) / TileSize) * TileSize - edge;
		break;
	case Down:
		edge = posY + (TileSize >> 1) + (TileSize & 0b1);
		tile = getTileAt(edge / TileSize, posX / TileSize);
		if (!isWalkable(tile))
			return edge - (edge / TileSize) * TileSize;
		break;
	case Left:
		edge = posX - (TileSize >> 1);
		tile = getTileAt(posY / TileSize, edge / TileSize);
		if (!isWalkable(tile))
			return (1 + (edge - 1) / TileSize) * TileSize - edge;
		break;
	case Right:
		edge = posX + (TileSize >> 1) + (TileSize & 0b1);
		tile = getTileAt(posY / TileSize, edge / TileSize);
		if (!isWalkable(tile))
			return edge - (edge / TileSize) * TileSize;
		break;
	}
	return 0;
}

pos Map::findTile(const int& posX, const int& posY) const
{
	pos result;
	result.first  = posX / TileSize;
	result.second = posY / TileSize;
	return result;
}

char Map::collectableCheck(const int& posX, const int& posY)
{
	auto tile = findTile(posX, posY);
	switch (getTileAt(tile.second, tile.first))
	{
	case Dot:
		setTileAt(tile.second, tile.first, Path);
		return Dot;
	case Pill:
		setTileAt(tile.second, tile.first, Path);
		return Pill;
	default:
		return '\0';
	}
}

bool Map::isPacmanWalkable(const int & col, const int & row, bool(*isWalkable)(const char&)) const
{
	const char& nextTile = getTileAt(col, row);
	return isWalkable(nextTile);
}

const unsigned char Map::randomTurn(const int& x, const int& y, const unsigned char & direction) const
{
	int random = rand();
	int directionCount = 0;
	unsigned char directions[4];

	availableTurns(x, y, directions, directionCount, direction, Ghost::isGhostWalkable);

	if (!directionCount)
		return direction;
	return directions[random % directionCount];
}

const unsigned char Map::chase(const int & x, const int & y, const pos & target, const unsigned char & direction, const int& movement, bool (*isWalkable)(const char&)) const
{
	bool mustTurn = false;
	switch (direction) {
	case Up:
		if (!(mustTurn = collisionWall(x, y - movement, Up, isWalkable)) && y - movement > target.second)
			return direction;
		break;
	case Down:
		if (!(mustTurn = collisionWall(x, y + movement, Down, isWalkable)) && y + movement < target.second)
			return direction;
		break;
	case Left:
		if (!(mustTurn = collisionWall(x - movement, y, Left, isWalkable)) && x - movement > target.first)
			return direction;
		break;
	case Right:
		if (!(mustTurn = collisionWall(x + movement, y, Right, isWalkable)) && x + movement < target.first)
			return direction;
		break;
	}

	int directionCount = 0;
	unsigned char directions[4];

	availableTurns(x, y, directions, directionCount, direction, isWalkable);

	if (mustTurn && directionCount == 1) 
		return directions[0];

	for (int i = 0; i < directionCount; ++i)
	{
		switch (directions[i]) {
		case Up:
			if (y >= target.second)
				return directions[i];
			break;
		case Down:
			if (y <= target.second)
				return directions[i];
			break;
		case Left:
			if (x >= target.first)
				return directions[i];
			break;
		case Right:
			if (x <= target.first)
				return directions[i];
			break;
		}
	}
	return direction;
}