#include "Troopers.h"

void Troopers::Init()
{
	posX = 4.5 * 64;
	posY = 6 * 64;
	posZ = 0;
	trooperSpritePtr[0] = new olc::Sprite("textures/trooper/trooperidle.png");
	for (int i = 0; i < amount; i++)
	{
		SpriteColorKey(orgPixel, newPixel, trooperSpritePtr[i]);
	}
	
}

void Troopers::Draw(olc::PixelGameEngine* PGEptr, float playerx, float playery, float pAngle)
{
	auto is_between_pi_factors = [=](float fAngle, float fFactorLow, float fFactorHigh) {

		return ((fAngle >= 3.14159f * fFactorLow) && (fAngle < 3.14159f * fFactorHigh)); };
	int x, y;
	float sx = posX - playerx;
	float sy = posY - playery;
	float sz = posZ;

	float CS = cos(pAngle), SN = sin(pAngle);
	float a = sy * CS + sx * SN;
	float b = sx * CS - sy * SN;
	sx = a; sy = b;
	sx = (sx * 108.0 / sy) + (120 / 2);
	sy = (sz * 108.0 / sy) + (80 / 2);
	fHeading = pAngle - angle + 3.14159f / 4.0f;
	if (fHeading < 0) fHeading += 2.0f * 3.14159f;
	if (fHeading >= 2.0f * 3.14159f) fHeading -= 2.0f * 3.14159f;

	int scale = 42 * 80 / b;

	//if (scale < 0) { scale = 0; } if (scale > 120) { scale = 120; }
	scale = std::max(0, std::min(PGEptr->ScreenWidth() / 2, scale));

	int nTextureSize = 200;
	olc::vi2d curTile = { 0,0 };
	olc::vi2d tileSize = { 200,200 };
	//textures
	float t_x = 0, t_y = 0, t_x_step = (float(nTextureSize) - 0.5f) / (float)scale, t_y_step = float(nTextureSize) / (float)scale;



	if (is_between_pi_factors(fHeading, 0.0f, 0.5f))
	{ //front
		curTile = { 0,0 };
	}
	if (is_between_pi_factors(fHeading, 0.5f, 1.0f))
	{ // left
		curTile = { 1,0 };
	}
	if (is_between_pi_factors(fHeading, 1.0f, 1.5f))
	{ //back
		curTile = { 2,0 };
	}

	if (is_between_pi_factors(fHeading, 1.5f, 2.0f))
	{ //right
		curTile = { 3,0 };
	}



	for (x = sx - scale / 2; x < sx + scale / 2; x++)
	{
		t_y = nTextureSize - 1;
		for (y = 0; y < scale; y++)
		{
			if (x > 0 && x < 120 && b < depth[x])
			{
				olc::Pixel samplePixel = trooperSpritePtr[0]->GetPixel(curTile.x * tileSize.x + t_x, curTile.y * tileSize.y + t_y);
				
					PGEptr->FillRect(x * 8, (sy - y) * 8, 8, 8, samplePixel);
				
				t_y -= t_y_step; if (t_y < 0) { t_y = 0; }
			}
		}
		t_x += t_x_step;
	}
}

void Troopers::SpriteColorKey(olc::Pixel& fromCol, olc::Pixel& toCol, olc::Sprite* sprPtr)
{
	for (int y = 0; y < sprPtr->height; y++)
	{
		for (int x = 0; x < sprPtr->width; x++)
		{
			if (sprPtr->GetPixel(x, y) == fromCol)
			{
				sprPtr->SetPixel(x, y, toCol);
			}
		}
	}
}
