#pragma once

#include <SDL2\SDL.h>
#include "Graphics.h"
#include "Map.h"

const unsigned char Up = 0b00;
const unsigned char Down = 0b01;
const unsigned char Left = 0b10;
const unsigned char Right = 0b11;
const unsigned char DirectionMask = 0b0011;
const unsigned char TurnMask = 0b1100;

class MobileObject
{
public:
	MobileObject(const int&, const int&);
	virtual ~MobileObject() {}
	virtual void move(Map&) = 0;
	void draw(SDL_Renderer*, const pos&);
	virtual void putOnTrack(const Map&);
	virtual void reset(const Map&);

	int posX, posY;
	int lastMoveTicks;
	unsigned char movementInfo;

	inline unsigned char MobileObject::getDirection() const
	{
		return (movementInfo & DirectionMask);
	}

	inline unsigned char MobileObject::getTurnDirection() const
	{
		return ((movementInfo & TurnMask) >> 2);
	}
protected:
	Animation* currentAnimation;
	int frameDistance(const int&) const;

	inline void MobileObject::setDirection(const unsigned char& direction)
	{
		movementInfo = ((movementInfo & TurnMask) | direction);
	}

	inline void MobileObject::setTurnDirection(const unsigned char& direction)
	{
		movementInfo = ((movementInfo & DirectionMask) | (direction << 2));
	}

};
