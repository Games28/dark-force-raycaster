#pragma once
#ifndef TROOPER_H
#define TROOPER_H

#include "NPC.h"
#define SCALE 2
extern int glbSliceWidth;
extern int depth[];
extern int World_Scale;

class Troopers : public NPC
{
public:
	Troopers() = default;
	void Init(olc::PixelGameEngine* PGEptr) override;
	void Draw(olc::PixelGameEngine* PGEptr, float playerx, float playery, float pAngle) override;

	void SpriteColorKey(olc::Pixel& fromCol, olc::Pixel& toCol, olc::Sprite* sprPtr) override;
private:
	
	olc::vf2d trooperPos;
	static const int amount = 1;
	olc::Sprite* trooperSpritePtr[amount];
	olc::Pixel orgPixel = olc::MAGENTA;
	olc::Pixel newPixel = olc::BLANK;
	float posX, posY, posZ, fHeading, angle;
	float fFOV = 3.14159f / 4.0f;
	olc::vi2d curTile; 
	olc::vi2d tileSize; 
	
};

#endif // !TROOPER_H
