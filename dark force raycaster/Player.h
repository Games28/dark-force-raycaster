#pragma once
#ifndef PLAYER_H
#define PLAYER_H


#include "olcPixelGameEngine.h"


class Player
{
public:
	Player() = default;
	void Init();
	void handSprite();
	void Draw(olc::PixelGameEngine *PGEptr);
public:
	float x, y;
	float pdx, pdy, angle;
	olc::vi2d handPos;
	static const int amount = 4;
	olc::Sprite *spriteHandPtr[amount];
};

#endif // !PLAYER_H