#pragma once

#include <string>
#include <SDL2\SDL.h>

using std::string;
typedef std::pair<int, int> pos;

const unsigned int WindowWidth  = 480;
const unsigned int WindowHeight = 640;
const unsigned int FPS = 60;
const unsigned int RandomSeed = 42;
	  
const unsigned int TileSize = 15;
const unsigned int PacmanMoveSpeed = 110;
const unsigned int GhostMoveSpeed = 100;

const unsigned int FrightenTime = 6000;
const unsigned int ScatterTime = 7000;

const unsigned int PinkyCagedTime = 0;
const unsigned int InkyCagedTime = 3000;
const unsigned int ClydeCagedTime = 6000;

const float DotSize = 3.0f;
const unsigned char DotColor[] = {255, 255, 255, 255};

const unsigned int PillSize = TileSize / 3;
const unsigned int PillBlinkTicks = 300;

const unsigned int IngameInfoTextSize = TileSize;
const SDL_Color IngameInfoTextColor = { 130, 130, 130, 255 };

const char InkyChar = 'i';
const char BlinkyChar = 'b';
const char ClydeChar = 'z';
const char PinkyChar = 'r';
const char PlayerChar = 's';
const char Tunel = 't';
const char Cage = 'c';
const char Door = 'd';
const char Outside = 'o';
const char Path = 'p';
const char Dot = 'P';
const char Pill = 'H';
const char InnerWall = 'w';
const char OuterWall = 'W';

const unsigned int DotPoints  = 10;
const unsigned int PillPoints = 50;
const unsigned int GhostPoints = 200;

struct AdditionalInfo 
{
	const char type;
	AdditionalInfo(const char& type) : type(type) {}
};

struct FontInfo : public AdditionalInfo
{
	FontInfo() : AdditionalInfo('F') {}
};

struct TextureInfo : public AdditionalInfo
{
	const SDL_Rect textureRect;
	TextureInfo(const SDL_Rect& textureRect) : AdditionalInfo('T'), textureRect(textureRect) {}
};

struct AnimationInfo : public AdditionalInfo
{
	const SDL_Rect* frames;
	const unsigned int framesCount;
	const unsigned int duration;
	AnimationInfo(const SDL_Rect* frames, const int& duration, const int& framesCount) : 
		AdditionalInfo('A'), 
		frames(frames), 
		framesCount(framesCount), 
		duration(duration) {}
};

struct MapInfo : public AdditionalInfo
{
	MapInfo() : AdditionalInfo('M') {}
};

struct Resource
{
	Resource(const char* relativePath, const int& id, const AdditionalInfo* info) : 
		relativePath(relativePath),
		id(id),
		info(info) {}
	const char* relativePath;
	const int id;
	const AdditionalInfo* info;
	string getAbsolutePath() const;
};

extern const Resource MMap;
extern const Resource FDefaultFont;

extern const Resource TCherry;
extern const Resource TLive;
extern const Resource TPill;
extern const Resource TWallInnerLeft;
extern const Resource TWallInnerRight;
extern const Resource TWallInnerTop;
extern const Resource TWallInnerBottom;
extern const Resource TCornerInnerTopRight;
extern const Resource TCornerInnerTopLeft;
extern const Resource TCornerInnerBottomRight;
extern const Resource TCornerInnerBottomLeft;
extern const Resource TCornerInnerTopRight2;
extern const Resource TCornerInnerTopLeft2;
extern const Resource TCornerInnerBottomRight2;
extern const Resource TCornerInnerBottomLeft2;
extern const Resource TCornerOuterTopRight;
extern const Resource TCornerOuterTopLeft;
extern const Resource TCornerOuterBottomRight;
extern const Resource TCornerOuterBottomLeft;
extern const Resource TCornerOuterTopRight2;
extern const Resource TCornerOuterTopLeft2;
extern const Resource TCornerOuterBottomRight2;
extern const Resource TCornerOuterBottomLeft2;
extern const Resource TCornerCageTopRight;
extern const Resource TCornerCageTopLeft;
extern const Resource TCornerCageBottomRight;
extern const Resource TCornerCageBottomLeft;
extern const Resource TWallOuterLeft;
extern const Resource TWallOuterRight;
extern const Resource TWallOuterTop;
extern const Resource TWallOuterBottom;
extern const Resource TDoorStepLeft;
extern const Resource TDoorStepRight;
extern const Resource TDoor;
extern const Resource TInOutTopRight;
extern const Resource TInOutTopLeft;
extern const Resource TInOutBottomRight;
extern const Resource TInOutBottomLeft;
extern const Resource TInOutRightTop;
extern const Resource TInOutLeftTop;
extern const Resource TInOutRightBottom;
extern const Resource TInOutLeftBottom;

extern const Resource APacmanWalk;      
extern const Resource AGhostPinkWalkUp;
extern const Resource AGhostPinkWalkDown;
extern const Resource AGhostPinkWalkLeft;
extern const Resource AGhostPinkWalkRight;
extern const Resource AGhostGreenWalkUp;
extern const Resource AGhostGreenWalkDown;
extern const Resource AGhostGreenWalkLeft;
extern const Resource AGhostGreenWalkRight;
extern const Resource AGhostYellowWalkUp;
extern const Resource AGhostYellowWalkDown;
extern const Resource AGhostYellowWalkLeft;
extern const Resource AGhostYellowWalkRight;
extern const Resource AGhostRedWalkUp;
extern const Resource AGhostRedWalkDown;
extern const Resource AGhostRedWalkLeft;
extern const Resource AGhostRedWalkRight;
extern const Resource AEyesUp;
extern const Resource AEyesDown;
extern const Resource AEyesLeft;
extern const Resource AEyesRight;
extern const Resource AFrightened;
extern const Resource AFrightenedEnd;