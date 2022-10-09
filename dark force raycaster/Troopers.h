#pragma once
#include "NPC.h"

class Troopers : public NPC
{
public:
	Troopers() = default;
	void Init() override;
	void Draw(olc::PixelGameEngine* PGEptr, float playerx, float playery, float pAngle) override;
	void SpriteColorKey(olc::Pixel& fromCol, olc::Pixel& toCol, olc::Sprite* sprPtr) override;
private:
	olc::vf2d trooperPos;
	static const int amount = 1;
	olc::Sprite* trooperSpritePtr[amount];
	olc::Pixel orgPixel = olc::MAGENTA;
	olc::Pixel newPixel = olc::BLANK;
	float posX, posY, posZ, fHeading, angle;
	int depth[120];
	olc::vi2d curTile; 
	olc::vi2d tileSize; 
};

