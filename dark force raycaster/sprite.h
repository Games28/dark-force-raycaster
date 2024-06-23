#ifndef SPRITE_H
#define SPRITE_H
#include "olcPixelGameEngine.h"
#include "map.h"
#include "player.h"
#include "ray.h"
#include "textures.h"

struct sprite_t
{
	float x, y;
	int texture;
	float distance;
	float rotationAngle;
	bool visible;
	float scale;
};

class Sprite
{
public:
	Sprite() = default;
	void Init_Sprites(Map* mapPtr);
	void renderSpriteProjection(olc::PixelGameEngine* pge, Player& player, Rays& ray);
	void renderMapSprites(olc::PixelGameEngine* pge);
	float distanceBetweenPoints(float x1, float y1, float x2, float y2);
	void normalizeAngle(float* angle);

public:
	Map* pmap = nullptr;
	sprite_t sprites[NUM_SPRITES];
	std::vector<olc::Sprite*> spritetextures;
	
	

};

#endif // !SPRITE_H

