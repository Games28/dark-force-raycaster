#include "ray.h"

void Rays::castRay(float rayAngle, int stripId, Player& player, Map& map, int level)
{

	normalizeAngle(&rayAngle);

	// determine whether ray is going up or down
	int isRayFacingDown = rayAngle > 0 && rayAngle < PI;
	int isRayFacingUp = !isRayFacingDown;
	// determine whether ray is going left or right
	int isRayFacingRight = rayAngle < 0.5 * PI || rayAngle > 1.5 * PI;
	int isRayFacingLeft = !isRayFacingRight;

	float xintercept, yintercept;
	float xstep, ystep;

	///////////////////////////////////////////
	// HORIZONTAL RAY-GRID INTERSECTION CODE
	///////////////////////////////////////////
	bool foundHorzWallHit = false;
	float horzWallHitX = 0;
	float horzWallHitY = 0;
	int horzWallContent = 0;
	float horzwallheight = 0.0f;

	// Find the y-coordinate of the closest horizontal grid intersection
	yintercept = floor(player.y / TILE_SIZE) * TILE_SIZE;
	yintercept += isRayFacingDown ? TILE_SIZE : 0;

	// Find the x-coordinate of the closest horizontal grid intersection

	xintercept = player.x + (yintercept - player.y) / tan(rayAngle);

	// Calculate the increment xstep and ystep
	ystep = TILE_SIZE;
	ystep *= isRayFacingUp ? -1 : 1;

	xstep = TILE_SIZE / tan(rayAngle);
	xstep *= (isRayFacingLeft && xstep > 0) ? -1 : 1;
	xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;

	float nextHorzTouchX = xintercept;
	float nextHorzTouchY = yintercept;

	// aux lambda to populate a hit record
	auto fill_hitrec = [=](ray_t& rec,float fheight, float fA, float fWhX, float fWhY, float fDst, bool bHitVer, int nTxtr, int level) {
		rec.rayAngle = fA;
		rec.wallHitX = fWhX;
		rec.wallHitY = fWhY;
		rec.front_distance = fDst;
		rec.wasHitVertical = bHitVer;
		rec.texture = nTxtr;
		rec.maplayer = level;
		rec.fHeight = fheight;
		};

	// clear the list of hit points before you start filling it again
	

	// Increment xstep and ystep until we find a non-transparent wall
	while (map.isInsideMap(nextHorzTouchX, nextHorzTouchY))
	{
		float xToCheck = nextHorzTouchX;
		if (level == 3)
		{
			int u = 0;
		}
		float yToCheck = nextHorzTouchY + (isRayFacingUp ? -1 : 0);
		
		//map.mapHasWallAt is working version
		if (map.test_map_has_wall_at(xToCheck, yToCheck, level)) {
			// found a wall hit
			horzWallHitX = nextHorzTouchX;
			horzWallHitY = nextHorzTouchY;
			//map.getMapAt is working version
			horzWallContent = 1;
			horzwallheight	= map.test_get_map_at((int)floor(yToCheck / TILE_SIZE), (int)floor(xToCheck / TILE_SIZE),level);
			foundHorzWallHit = true;
			float horzHitDistance = foundHorzWallHit
				? distanceBetweenPoints(player.x, player.y, horzWallHitX, horzWallHitY)
				: FLT_MAX;


			ray_t tmp_hitpoint;
			fill_hitrec(tmp_hitpoint,horzwallheight, rayAngle, horzWallHitX, horzWallHitY, horzHitDistance, false, horzWallContent,level - 1);
			rays[stripId].push_back(tmp_hitpoint);
		}
		nextHorzTouchX += xstep;
		nextHorzTouchY += ystep;
	}

	///////////////////////////////////////////
	// VERTICAL RAY-GRID INTERSECTION CODE
	///////////////////////////////////////////
	bool foundVertWallHit = false;
	float vertWallHitX = 0;
	float vertWallHitY = 0;
    int vertWallContent = 0;
	float vertwallheight = 0.0f;

	// Find the x-coordinate of the closest horizontal grid intersection
	xintercept = floor(player.x / TILE_SIZE) * TILE_SIZE;
	xintercept += isRayFacingRight ? TILE_SIZE : 0;

	// Find the y-coordinate of the closest horizontal grid intersection
	float e = tan(rayAngle);
	float a = xintercept - player.x;
	float b = a * e;
	float c = player.y + b;
	yintercept = c;

	// Calculate the increment xstep and ystep
	xstep = TILE_SIZE;
	xstep *= isRayFacingLeft ? -1 : 1;

	ystep = TILE_SIZE * tan(rayAngle);
	ystep *= (isRayFacingUp && ystep > 0) ? -1 : 1;
	ystep *= (isRayFacingDown && ystep < 0) ? -1 : 1;

	float nextVertTouchX = xintercept;
	float nextVertTouchY = yintercept;


	// Increment xstep and ystep until we find a wall
	while (map.isInsideMap(nextVertTouchX, nextVertTouchY)) {
		float xToCheck = nextVertTouchX + (isRayFacingLeft ? -1 : 0);
		float yToCheck = nextVertTouchY;
		
		//map.mapHasWallAt is working version
		if (map.test_map_has_wall_at(xToCheck, yToCheck,level)) {
			// found a wall hit
			vertWallHitX = nextVertTouchX;
			vertWallHitY = nextVertTouchY;

			//map.getMapAt is working version
			vertWallContent = 1;
			vertwallheight = map.test_get_map_at((int)floor(yToCheck / TILE_SIZE), (int)floor(xToCheck / TILE_SIZE),level);
			foundVertWallHit = true;
			float vertHitDistance = foundVertWallHit
				? distanceBetweenPoints(player.x, player.y, vertWallHitX, vertWallHitY)
				: FLT_MAX;


			ray_t tmp_hitpoint;
			fill_hitrec(tmp_hitpoint,vertwallheight, rayAngle, vertWallHitX, vertWallHitY, vertHitDistance, true, vertWallContent, level - 1);
			rays[stripId].push_back(tmp_hitpoint);
		}
		nextVertTouchX += xstep;
		nextVertTouchY += ystep;
	}

	
}

void Rays::castAllRays(Player& player, Map& map)
{
	
	//for loop for levels
	
		for (int col = 0; col < NUM_RAYS; col++) 
		{
			//clear current ray array list for new update info
			rays[col].clear();


			for (int i = 1; i < 4; i++)
			{
			   float rayAngle = player.rotationAngle + (float)(col - NUM_RAYS / 2) / (float)(NUM_RAYS)*FOV_ANGLE;
			   castRay(rayAngle, col, player, map,i);
		    }


			std::sort(
				rays[col].begin(),
				rays[col].end(),
				[](ray_t& a, ray_t& b)
				{
					return (a.front_distance > b.front_distance) ||
						(a.front_distance == b.front_distance && a.maplayer < b.maplayer);
				}
			);

			rays[col].erase(
				std::remove_if(
					rays[col].begin(),
					rays[col].end(),
					[](ray_t& a) {
						return a.fHeight == 0.0f;
					}
				),
				rays[col].end()
			);
			
	    }
}

void Rays::renderMapRays(olc::PixelGameEngine* pge, Player& player)
{
	for (int i = 0; i < NUM_RAYS; i++) {
		pge->DrawLine(
			(int)(player.x * MINIMAP_SCALE_FACTOR),
			(int)(player.y * MINIMAP_SCALE_FACTOR),
			(int)(rays[i][0].wallHitX * MINIMAP_SCALE_FACTOR),
			(int)(rays[i][0].wallHitY * MINIMAP_SCALE_FACTOR),
			olc::CYAN
		);
	}
}

void Rays::normalizeAngle(float* angle)
{
	*angle = remainder(*angle, TWO_PI);
	if (*angle < 0) {
		*angle = TWO_PI + *angle;
	}
	
}

float Rays::distanceBetweenPoints(float x1, float y1, float x2, float y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
