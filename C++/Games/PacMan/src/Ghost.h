#pragma once

#include "Map.h"
#include "MobileObject.h"

enum Mode;

class Ghost : public MobileObject 
{
public:
	Ghost(const int&, const int&);
	void move(Map&);
	static void loadAnimations();
	static bool isGhostWalkable(const char&);
	void putOnTrack(const Map&);
	void frighten();

	pos lastTurnTile;
	static int scatterTimer;
protected:
	void frightenedDirection(const Map&, const int&);
	void exitDirection(const Map&);
	void direction(const Map&, const int&, bool(*)(const char&));
	void collidePlayer();
	bool reachedTarget() const;
	virtual void chase() = 0;
	virtual void changeAnimation() = 0;
	virtual int getSpeed() const = 0;
	void reverseDirection();

	static bool isCagedWalkable(const char&);
	static bool isScatterWalkable(const char&);
	static bool isEatenWalkable(const char&);

	static Animation animations[];
	Mode mode;
	pos target;
	pos scatter;
	pos eatenTarget;
	int frightenedTimer;
	int cagedTimer;
	bool caged;
};

class Pinky : public Ghost
{
public:
	Pinky(const int&, const int&, const Map&);
	static void loadAnimations();
	void reset(const Map&);
private:
	void changeAnimation();
	void chase();
	int getSpeed() const;
	static Animation animations[];
};

class Clyde : public Ghost
{
public:
	Clyde(const int&, const int&, const Map&);
	static void loadAnimations();
	void reset(const Map&);
private:
	void changeAnimation();
	void chase();
	int getSpeed() const;
	static Animation animations[];
};

class Inky : public Ghost
{
public:
	Inky(const int&, const int&, const Map&);
	static void loadAnimations();
	void reset(const Map&);
private:
	void changeAnimation();
	void chase();
	int getSpeed() const;
	static Animation animations[];
};

class Blinky : public Ghost
{
public:
	Blinky(const int&, const int&, const Map&);
	static void loadAnimations();
	void reset(const Map&);
private:
	void changeAnimation();
	void chase();
	int getSpeed() const;
	static Animation animations[];
};