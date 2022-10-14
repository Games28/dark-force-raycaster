#pragma once
#ifndef LIGHTSABER_H
#define LIGHTSABER_H


#include "olcPixelGameEngine.h"
#define SCALE 2
extern int depth[];
extern int glbSliceWidth;

class Lightsaber
{
public:
	Lightsaber() = default;
	void Init();
	void Draw(olc::PixelGameEngine* PGEptr, float deltatime, float playerx, float playery);

	void SpriteColorKey(olc::Pixel& fromCol, olc::Pixel& toCol, olc::Sprite* sprPtr);
	

private:
	olc::vi2d saberposDefault = { 520,240 };
	olc::vi2d saberposLeft = { 0,240 };
	olc::vi2d saberposRight = { 670,240 };
	
	olc::Sprite* sprites[5];
	float fTimer = 0.0f;
	int nFrameCnt = 0;
	float fFrameTime = 0.3f;
	float SFrameTime = 0.1f;
	int nScale = SCALE;
	int nOffsetX = 0;
	int nOffsetY = 0;
	bool animation = false;
	bool animationLeft = false;
	bool animationRight = false;
	olc::Pixel orgPixel = olc::MAGENTA;
	olc::Pixel newPixel = olc::BLANK;

};

#endif // !LIGHTSABER_H