#ifndef RAY_H
#define RAY_H
#include "olcPixelGameEngine.h"
#include "map.h"
#include "player.h"
#include "vector.h"
#include "textures.h"

struct ray_t
{
	float rayAngle;
	float wallHitX;
	float wallHitY;
	
	bool wasHitVertical;
	bool firstWall;
	int texture;
	int maplayer = -1;

	//hit blocks
	float front_distance,
		Back_distance;

	int bottom_front = -1;
	int Bottom_back = -1;
	int top_front = -1;
	int top_back = -1;
};


struct ray_levels
{
	std::vector<ray_t> HitListType;
};
//typedef std::vector<ray_t> HitListType;

class Rays
{
public:
	Rays() = default;
	void castRay(float rayAngle, int stripId, Player& player, Map& map, int level);
	void castAllRays(Player& player, Map& map);
	void renderMapRays(olc::PixelGameEngine* pge, Player& player);
	void normalizeAngle(float* angle);
	float distanceBetweenPoints(float x1, float y1, float x2, float y2);


public:

	ray_levels rays[NUM_RAYS];
	
	//rays array per level
	
};

#endif // !RAY_H

