#include "Player.h"

void Player::Init()
{
	x = 150; y = 400; angle = 90 * 3.14159f / 180.0f; 
	pdx = cos(angle); pdy = (angle);
	handPos = { 100,350 };

}

void Player::handSprite()
{
	spriteHandPtr[0] = new olc::Sprite("textures/Dhand.png");
	spriteHandPtr[1] = new olc::Sprite("textures/Ghand.png");
	spriteHandPtr[2] = new olc::Sprite("textures/Lhand.png");
}

void Player::Draw(olc::PixelGameEngine* PGEptr)
{
	float sx = handPos.x;
	float sy = handPos.y;
	int x, y;
	int textureSize = 100;

	for (y = 0; y < textureSize; y++)
	{
		for (x = 0; x < textureSize; x++)
		{

			olc::Pixel samplePixel = spriteHandPtr[2]->GetPixel(x, y);
			if (samplePixel != olc::MAGENTA)
			{
				PGEptr->FillRect(sx + x * 4, sy + y * 4, 4, 4, samplePixel);
			}
		}
	}
}
