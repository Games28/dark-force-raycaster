#include "Troopers.h"

void Troopers::Init(olc::PixelGameEngine* PGEptr)
{
	posX = 4.5 * 64;
	posY = 6 * 64;
	posZ = 25;
	angle = 0;
	
	trooperSpritePtr[0] = new olc::Sprite("textures/trooper/newtrooperidle.png");
	//for (int i = 0; i < amount; i++)
	//{
	//	SpriteColorKey(orgPixel, newPixel, trooperSpritePtr[i]);
	//}
	
}

void Troopers::Draw(olc::PixelGameEngine* PGEptr, float playerx, float playery, float pAngle)
{

	auto is_between_pi_factors = [=](float fAngle, float fFactorLow, float fFactorHigh) {
		// Joseph21 - if you have PI defined as a constant, then use PI iso 3.14159f
		return ((fAngle >= 3.14159f * fFactorLow) && (fAngle < 3.14159f * fFactorHigh));
	};

	float sx = posX - playerx;
	float sy = posY - playery;
	float sz = posZ;
	float x, y;
	float fEyeX = cosf(pAngle);
	float fEyeY = sinf(pAngle);
	int nTextureSize = 200;
	olc::vi2d curTile = { 0,0 };
	olc::vi2d tileSize = { 130, 130 };

	float  fObjectAngle = -atan2f(sy, sx) - atan2f(fEyeY, fEyeX);
	fHeading = pAngle - angle + 3.14159f / 4.0f;
	if (fHeading < 0) fHeading += 2.0f * 3.14159f;
	if (fHeading >= 2.0f * 3.14159f) fHeading -= 2.0f * 3.14159f;
	//		
	if (fObjectAngle < -3.14159f) fObjectAngle += 2.0f * 3.14159f;
	if (fObjectAngle > 3.14159f) fObjectAngle -= 2.0f * 3.14159f;

	bool bInPlayerFOV = fabs(fObjectAngle) < fFOV / 2.0f;

	float fDistanceFromPlayer = sqrtf(sx * sx + sy * sy);

	int CellSize = 64;
	int sliceWidth = 0;
	int panewidth = 0;
	int paneheight = 0;

	float fObjectCeiling = 0;
	float fObjectFloor = 0;
	float fObjectHeight = 0;
	float fObjectAspectRatio = 0;
	float fObjectWidth = 0;
	float fMiddleOfObject = 0;

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

	// Joseph21 - introduced scale factor to choose right ratio between object size and surrounding world
	//            [ this appears to distort the rendering though ... :( ]
	float fObjectScale = 0.65f;    // this turns out to be ok for r2d2

	//		if (bInPlayerFOV && fDistanceFromPlayer >= 5.0f && fDistanceFromPlayer < ndepth)
			// Joseph21 - the check against ndepth is not useful here - depth checking is done per column within the horizontal loop
	if (bInPlayerFOV && fDistanceFromPlayer >= 0.5f * CellSize)
	{
		//			sliceWidth = 8;
		sliceWidth = 2;    // Joseph21 - please don't use hardcoded constants if you have defined symbolic ones
		//            suggestion: you could use only the constant and remove the variable
		panewidth = PGEptr->ScreenWidth() / sliceWidth;
		paneheight = PGEptr->ScreenHeight() / sliceWidth;

		fObjectCeiling = (float)(paneheight / 2) - fObjectScale * paneheight / ((float)fDistanceFromPlayer / float(CellSize));
		fObjectFloor = paneheight - fObjectCeiling;
		fObjectHeight = fObjectFloor - fObjectCeiling;
		fObjectAspectRatio = tileSize.y / tileSize.x;
		fObjectWidth = fObjectHeight / fObjectAspectRatio;
		fMiddleOfObject = (0.5f * (-fObjectAngle / (fFOV / 2.0f)) + 0.5f) * (float)panewidth;
		
		for (x = 0; x < fObjectWidth; x++)
		{
			// Joseph21 - you only need to calculate the object column once to iterate vertically over the pixels
			//int nObjectColumn = (int)(fMiddleOfObject + x - (fObjectWidth / 2.0f));
			// Joseph21 - only iterate this column if object is closer than wall
			//if (nObjectColumn >= 0 && nObjectColumn < panewidth && depth[nObjectColumn] >= fDistanceFromPlayer)
			{
				for (y = 0; y < fObjectHeight; y++)
				{
					// Joseph21 - take into account that you have multiple subsprites in one sprite sheet
					int nSelectedSubSprite = curTile.x;
					int nSubSpritesX = 4;
					float fSpriteFraction = 1.0f / float(nSubSpritesX);
					//// Joseph21 - determine sample coordinate depending on selected sub sprite and number of subsprites in sprite file
					float fSampleX = (x / fObjectWidth) * fSpriteFraction + nSelectedSubSprite * fSpriteFraction;
					float fSampleY = y / fObjectHeight;
					int nObjectColumn = (int)(fMiddleOfObject + x - (fObjectWidth / 2.0f));


					olc::Pixel samplePixel = trooperSpritePtr[0]->Sample(fSampleX, fSampleY);
					
					if (samplePixel != olc::MAGENTA && nObjectColumn >= 0 && nObjectColumn < panewidth && depth[nObjectColumn] >= fDistanceFromPlayer)
					{
						

							// Joseph21 - use nObjectColumn to determine screen location for this pixel
							PGEptr->FillRect(nObjectColumn * sliceWidth, (fObjectCeiling + y) * sliceWidth, sliceWidth, sliceWidth, samplePixel);
							
							// Joseph21 - distance checking is moved outside this column loop
							depth[nObjectColumn] = fDistanceFromPlayer;
							
					}
					//depth[nObjectColumn] = fDistanceFromPlayer;

				}
				
				//DrawString(10, 10, "depth " + std::to_string(depth[nObjectColumn]));
			}
			
		}
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
