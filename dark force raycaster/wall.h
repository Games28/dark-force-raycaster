#ifndef WALL_H
#define WALL_H
#include "olcPixelGameEngine.h"
#include "player.h"
#include "ray.h"
#include "map.h"
#include "defs.h"

enum Sides
{
	TOP,
	BOTTOM,
	ROOF,
	WALL
};
struct sDelayedPixel
{
	int x;
	int y;
	olc::Pixel p;
};

typedef struct sDelayedPixel DelayedPixel;


class Wall
{
public:
	Wall() = default;
	void Init_texture();
	//lc::Pixel SelectSceneryPixel(int texture, int samplex, int sampley, Player& player, Sides side);
	void CalculateWallBottomAndTop2(float fCorrectedDistToWall, int nHorHeight, int nLevelHeight, 
		float fWallHeight, int& nWallTop, int& nWallBottom, Player& player);
	void changeColorIntensity(olc::Pixel* color, float factor);
	void renderWallProjection(olc::PixelGameEngine* pge, Rays& ray, Player& player);

	std::vector<olc::Sprite*> walltextures;

	

	std::vector<bool> isTranparent;

	
};
#endif // !WALL_H


