#include <SDL2\SDL.h>
#include "Constants.h"

#define COUNT_OF(x) sizeof(x) / sizeof(x[0]) 

// TODO: Automate this
static const TextureInfo IMapTexture({ 0, 0, 50, 50 });
const Resource TWallInnerLeft("Textures\\inner_side_wall_left.png",   10, &IMapTexture);
const Resource TWallInnerRight("Textures\\inner_side_wall_right.png",  11, &IMapTexture);
const Resource TWallInnerTop("Textures\\inner_side_wall_top.png",    12, &IMapTexture);
const Resource TWallInnerBottom("Textures\\inner_side_wall_bottom.png", 13, &IMapTexture);

const Resource TCornerInnerTopRight("Textures\\inner_corner_top_right.png",    14, &IMapTexture);
const Resource TCornerInnerTopLeft("Textures\\inner_corner_top_left.png",     15, &IMapTexture);
const Resource TCornerInnerBottomRight("Textures\\inner_corner_bottom_right.png", 16, &IMapTexture);
const Resource TCornerInnerBottomLeft("Textures\\inner_corner_bottom_left.png",  17, &IMapTexture);

const Resource TCornerInnerTopRight2("Textures\\inner_corner_top_right2.png", 18, &IMapTexture);
const Resource TCornerInnerTopLeft2("Textures\\inner_corner_top_left2.png", 19, &IMapTexture);
const Resource TCornerInnerBottomRight2("Textures\\inner_corner_bottom_right2.png", 20, &IMapTexture);
const Resource TCornerInnerBottomLeft2("Textures\\inner_corner_bottom_left2.png", 21, &IMapTexture);

const Resource TWallOuterLeft("Textures\\outer_side_wall_left.png", 22, &IMapTexture);
const Resource TWallOuterRight("Textures\\outer_side_wall_right.png", 23, &IMapTexture);
const Resource TWallOuterTop("Textures\\outer_side_wall_top.png", 24, &IMapTexture);
const Resource TWallOuterBottom("Textures\\outer_side_wall_bottom.png", 25, &IMapTexture);

const Resource TCornerOuterTopRight("Textures\\outer_corner_top_right.png", 26, &IMapTexture);
const Resource TCornerOuterTopLeft("Textures\\outer_corner_top_left.png", 27, &IMapTexture);
const Resource TCornerOuterBottomRight("Textures\\outer_corner_bottom_right.png", 28, &IMapTexture);
const Resource TCornerOuterBottomLeft("Textures\\outer_corner_bottom_left.png", 29, &IMapTexture);

const Resource TDoor("Textures\\door.png", 30, &IMapTexture);
const Resource TDoorStepLeft("Textures\\door_step_left.png", 31, &IMapTexture);
const Resource TDoorStepRight("Textures\\door_step_right.png", 32, &IMapTexture);

const Resource TCornerCageTopRight("Textures\\cage_corner_top_right.png", 33, &IMapTexture);
const Resource TCornerCageTopLeft("Textures\\cage_corner_top_left.png", 34, &IMapTexture);
const Resource TCornerCageBottomRight("Textures\\cage_corner_bottom_right.png", 35, &IMapTexture);
const Resource TCornerCageBottomLeft("Textures\\cage_corner_bottom_left.png", 36, &IMapTexture);

const Resource TCornerOuterTopRight2("Textures\\outer_corner_top_right2.png", 37, &IMapTexture);
const Resource TCornerOuterTopLeft2("Textures\\outer_corner_top_left2.png", 38, &IMapTexture);
const Resource TCornerOuterBottomRight2("Textures\\outer_corner_bottom_right2.png", 39, &IMapTexture);
const Resource TCornerOuterBottomLeft2("Textures\\outer_corner_bottom_left2.png", 40, &IMapTexture);

const Resource TInOutTopRight("Textures\\inouter_corner_top_right.png", 41, &IMapTexture);
const Resource TInOutTopLeft("Textures\\inouter_corner_top_left.png", 42, &IMapTexture);
const Resource TInOutBottomRight("Textures\\inouter_corner_bottom_right.png", 43, &IMapTexture);
const Resource TInOutBottomLeft("Textures\\inouter_corner_bottom_left.png", 44, &IMapTexture);
				
const Resource TInOutRightTop("Textures\\inouter_corner_right_top.png", 45, &IMapTexture);
const Resource TInOutLeftTop("Textures\\inouter_corner_left_top.png", 46, &IMapTexture);
const Resource TInOutRightBottom("Textures\\inouter_corner_right_bottom.png", 47, &IMapTexture);
const Resource TInOutLeftBottom("Textures\\inouter_corner_left_bottom.png", 48, &IMapTexture);

static const SDL_Rect pacmanWalkAnimation[] = {
	{ 3,   3, 45, 45 },
	{ 53,  3, 44, 44 },
	{ 103, 3, 44, 44 },
	{ 153, 3, 44, 44 },
	{ 203, 3, 44, 44 },
	{ 253, 3, 44, 44 },
	{ 303, 3, 44, 44 },
	{ 353, 3, 44, 44 },
	{ 403, 3, 44, 44 },
	{ 453, 3, 44, 44 },
	{ 503, 3, 44, 44 },
	{ 553, 3, 44, 44 },
	{ 603, 3, 44, 44 },
	{ 653, 3, 44, 44 },
	{ 703, 3, 44, 44 },
	{ 753, 3, 44, 44 },
	{ 803, 3, 44, 44 },
	{ 853, 3, 44, 44 },
};

static const SDL_Rect ghostWalkAnimationSingle[] = {
	{ 0,   0, 112, 112 },
};

static const SDL_Rect ghostWalkAnimation[] = {
	{ 0,   0, 111, 112 },
	{ 111,  0, 111, 112 }
};

static const SDL_Rect ghostWalkAnimationDouble[] = {
	{ 0,   0, 111, 112 },
	{ 112, 0, 111, 112 },
	{ 224, 0, 111, 112 },
	{ 336, 0, 111, 112 }
};

static const TextureInfo ICherry({ 0, 0, 56, 56 });
static const TextureInfo IPill({ 0, 0, 300, 300 });
static const TextureInfo ILive({ 0, 0, 40, 42 });

const Resource TCherry("Textures\\cherry.png", 1, &ICherry);
const Resource TLive("Textures\\live.png", 2, &ILive);
const Resource TPill("Textures\\pill.png", 3, &IPill);

static const MapInfo IMap;
const Resource MMap("Maps\\level_0.txt", 6, &IMap);

static const FontInfo IFont;
const Resource FDefaultFont("Fonts\\default.ttf", 101, &IFont);

static const AnimationInfo IPacmanWalk(pacmanWalkAnimation, 250, COUNT_OF(pacmanWalkAnimation));
static const AnimationInfo IGhostWalk(ghostWalkAnimation, 250, COUNT_OF(ghostWalkAnimation));
static const AnimationInfo IGhostWalkDouble(ghostWalkAnimationDouble, 500, COUNT_OF(ghostWalkAnimationDouble));
static const AnimationInfo IEyesWalk(ghostWalkAnimationSingle, INT_MAX, COUNT_OF(ghostWalkAnimationSingle));

const Resource APacmanWalk("Animations\\pacman_walk.png", 50, &IPacmanWalk);
const Resource AGhostPinkWalkUp("Animations\\ghost_pink_walk_up.png", 51, &IGhostWalk);
const Resource AGhostPinkWalkDown("Animations\\ghost_pink_walk_down.png", 52, &IGhostWalk);
const Resource AGhostPinkWalkLeft("Animations\\ghost_pink_walk_left.png", 53, &IGhostWalk);
const Resource AGhostPinkWalkRight("Animations\\ghost_pink_walk_right.png", 54, &IGhostWalk);
const Resource AGhostGreenWalkUp("Animations\\ghost_green_walk_up.png", 55, &IGhostWalk);
const Resource AGhostGreenWalkDown("Animations\\ghost_green_walk_down.png", 56, &IGhostWalk);
const Resource AGhostGreenWalkLeft("Animations\\ghost_green_walk_left.png", 57, &IGhostWalk);
const Resource AGhostGreenWalkRight("Animations\\ghost_green_walk_right.png", 58, &IGhostWalk);
const Resource AGhostYellowWalkUp("Animations\\ghost_yellow_walk_up.png", 59, &IGhostWalk);
const Resource AGhostYellowWalkDown("Animations\\ghost_yellow_walk_down.png", 60, &IGhostWalk);
const Resource AGhostYellowWalkLeft("Animations\\ghost_yellow_walk_left.png", 61, &IGhostWalk);
const Resource AGhostYellowWalkRight("Animations\\ghost_yellow_walk_right.png", 62, &IGhostWalk);
const Resource AGhostRedWalkUp("Animations\\ghost_red_walk_up.png", 63, &IGhostWalk);
const Resource AGhostRedWalkDown("Animations\\ghost_red_walk_down.png", 64, &IGhostWalk);
const Resource AGhostRedWalkLeft("Animations\\ghost_red_walk_left.png", 65, &IGhostWalk);
const Resource AGhostRedWalkRight("Animations\\ghost_red_walk_right.png", 66, &IGhostWalk);
const Resource AEyesUp("Animations\\eyes_up.png"  , 67, &IEyesWalk);
const Resource AEyesDown("Animations\\eyes_down.png", 68, &IEyesWalk);
const Resource AEyesLeft("Animations\\eyes_left.png", 69, &IEyesWalk);
const Resource AEyesRight("Animations\\eyes_right.png", 70, &IEyesWalk);
const Resource AFrightened("Animations\\ghost_frightened.png", 71, &IGhostWalk);
const Resource AFrightenedEnd("Animations\\ghost_frightened_end.png", 72, &IGhostWalkDouble);

string Resource::getAbsolutePath() const
{
	static string baseRes;
	if (baseRes.empty()) {
		char *basePath = SDL_GetBasePath();
		if (basePath) {
			baseRes = basePath;
			SDL_free(basePath);
		}
		else {
			printf("Error getting resource path: %s\n", SDL_GetError());
			exit(1);
		}

		baseRes += "Resources\\";
	}

	return std::move(baseRes + relativePath);
}
