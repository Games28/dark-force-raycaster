#pragma once
#include "olcPixelGameEngine.h"


class NPC
{
public:
	virtual void Init() = 0;
	virtual void Draw(olc::PixelGameEngine* PGEptr, float playerx, float playery, float pAngle) = 0;
	virtual void SpriteColorKey(olc::Pixel& fromCol, olc::Pixel& toCol, olc::Sprite* sprPtr) = 0;
};

