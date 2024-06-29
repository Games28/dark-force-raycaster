#include "player.h"

void Player::Init(Map* mapPtr)
{
	x = (float)WINDOW_WIDTH / 4.0f;
	y = (float)WINDOW_HEIGHT / 4.0f;
	width = 2;
	height = 2;
	turnDirection = 0;
	walkDirection = 0;
	rotationAngle = PI / 2.0f;
	walkSpeed = 100.0f;
	strafeSpeed = 50.0f;
	turnSpeed = 45.0f * (PI / 180.0f);
	strafeDirection = 0;
	pMap = mapPtr;
	
	fDepthBuffer = new float[WINDOW_WIDTH * WINDOW_HEIGHT];
}

void Player::move(float deltatime)
{
	rotationAngle += turnDirection * turnSpeed * deltatime;
	normalizeAngle(&rotationAngle);

	float moveStep = walkDirection * walkSpeed * deltatime;

	int new_index = pMap->get_table_index(rotationAngle);
	//float newPlayerX = x + cos(rotationAngle) * moveStep;
	//float newPlayerY = y + sin(rotationAngle) * moveStep;
	float newPlayerX = x + pMap->cos_table[new_index] * moveStep;
	float newPlayerY = y + pMap->sin_table[new_index] * moveStep;

	if (!pMap->mapHasWallAt(newPlayerX, newPlayerY)) {
		x = newPlayerX;
		y = newPlayerY;
		
	}
}

void Player::strafe(float deltatime)
{
	float strafestep = strafeSpeed * deltatime;
	int new_index = pMap->get_table_index(rotationAngle);
	float newStrafeX = 0;
	float newStrafeY = 0;
	if (strafeDirection == -1)
	{
		//newStrafeX = x - sin(rotationAngle) * strafestep;
		//newStrafeY = y + cos(rotationAngle) * strafestep;
		newStrafeX = x - pMap->sin_table[new_index] * strafestep;
		newStrafeY = y + pMap->cos_table[new_index] * strafestep;
	}
	if (strafeDirection == 1)
	{
		//newStrafeX = x + sin(rotationAngle) * strafestep;
		//newStrafeY = y - cos(rotationAngle) * strafestep;
		newStrafeX = x + pMap->sin_table[new_index] * strafestep;
		newStrafeY = y - pMap->cos_table[new_index] * strafestep;
	}

	if (!pMap->mapHasWallAt(newStrafeX, newStrafeY))
	{
		x = newStrafeX;
		y = newStrafeY;
	
	}
}

void Player::render(olc::PixelGameEngine* pge)
{
	
	pge->FillRect(
		(int)(x * MINIMAP_SCALE_FACTOR),
		(int)(y * MINIMAP_SCALE_FACTOR),
		(int)(width * MINIMAP_SCALE_FACTOR),
		(int)(height * MINIMAP_SCALE_FACTOR),
		olc::GREEN
	);
}

void Player::normalizeAngle(float* angle)
{
	*angle = remainder(*angle, TWO_PI);
	if (*angle < 0) {
		*angle = TWO_PI + *angle;
	}
	
}

float Player::fMaxDistance()
{
	return sqrt(MAP_NUM_COLS_X * MAP_NUM_COLS_X + MAP_NUM_ROWS_Y * MAP_NUM_ROWS_Y);
}

void Player::DrawDepth(olc::PixelGameEngine* pge, float fdepth, int x, int y, olc::Pixel color)
{
	if (x >= 0 && x < WINDOW_WIDTH &&
		y >= 0 && y < WINDOW_HEIGHT)
	{
		if (fdepth <= fDepthBuffer[y * WINDOW_WIDTH + x])
		{
			fDepthBuffer[y * WINDOW_WIDTH + x] = fdepth;
			pge->Draw(x, y, color);
		}
	}
}
