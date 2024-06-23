#ifndef WALL_H
#define WALL_H
#include "olcPixelGameEngine.h"
#include "player.h"
#include "ray.h"
#include "map.h"


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
	void changeColorIntensity(olc::Pixel* color, float factor);
	void renderWallProjection(olc::PixelGameEngine* pge, Rays& ray, Player& player);

	std::vector<olc::Sprite*> walltextures;

	

	std::vector<bool> isTranparent;

	
};
#endif // !WALL_H


