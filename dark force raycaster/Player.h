#ifndef PLAYER_H
#define PLAYER_H
#include "vector.h"
#include "olcPixelGameEngine.h"
#include "map.h"


class Player
{
public:
	Player() = default;
	void Init(Map* mapPtr);
	void move(float deltatime);
	void strafe(float deltatime);
	void render(olc::PixelGameEngine* pge);
	void normalizeAngle(float* angle);
	float fMaxDistance();
	void DrawDepth(olc::PixelGameEngine* pge, float fdepth, int x, int y, olc::Pixel color);

public:
	float x;
	float y;
	float width;
	float height;
	int   turnDirection; // -1 for left, +1 for right
	int   walkDirection; // -1 for back, +1 for front
	int   strafeDirection;
	float rotationAngle;
	float walkSpeed;
	float turnSpeed;
	float strafeSpeed;
	bool  isstrafingleft;
	bool  isstrafingright;
	float fPlayerH = 0.5f;


	Vec2 playermovebefore = Vec2(0, 0);
	Vec2 playermoveafter = Vec2(0, 0);
	Vec2 movedifference = Vec2(0, 0);
	float anglebefore = 0.0f;
	float angleaftere = 0.0f;
	float angledifference = 0.0f;

	Vec2 playerstrafebefore = Vec2(0, 0);
	Vec2 Playerstrafeafter = Vec2(0, 0);
	Vec2 stafedifference = Vec2(0, 0);

	//	Map map;
	Map* pMap = nullptr;

	float fmaxDist;
	float* fDepthBuffer = nullptr;
};

#endif // 


