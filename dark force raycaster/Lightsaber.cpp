#include "Lightsaber.h"

void Lightsaber::Init()
{
	sprites[0] = new olc::Sprite("textures/weapon/saberbackground.png");
	sprites[1] = new olc::Sprite("textures/weapon/newsaberA.png");
	sprites[2] = new olc::Sprite("textures/weapon/newsaberB.png");
	sprites[3] = new olc::Sprite("textures/weapon/saberglow.png");
	sprites[4] = new olc::Sprite("textures/weapon/saberglowtest.png");
	//sprites[4] = new olc::Sprite("textures/B1.5test.png");

	for (int i = 0; i < 4; i++)
	{
		SpriteColorKey(orgPixel, newPixel, sprites[i]);
	}
}

void Lightsaber::Draw(olc::PixelGameEngine* PGEptr, float deltatime, float playerx, float playery)
{
	PGEptr->SetPixelMode(olc::Pixel::MASK);
	int x, y;
	int texturesize = 0;
	if (PGEptr->GetKey(olc::Key::UP).bHeld) fFrameTime += 0.03f * deltatime;
	if (PGEptr->GetKey(olc::Key::DOWN).bHeld) fFrameTime -= 0.03f * deltatime;
	//if (PGEptr->GetKey(olc::Key::T).bPressed) animation = true;
	if (PGEptr->GetKey(olc::Key::LEFT).bPressed && animationRight == false) animationLeft = true;
	if (PGEptr->GetKey(olc::Key::RIGHT).bPressed && animationLeft == false) animationRight = true;
	if (animationLeft == true)
	{
		animationRight = false;
		fTimer += deltatime;


		PGEptr->DrawPartialSprite(saberposLeft.x, saberposLeft.y, sprites[2], nFrameCnt * 200, 0, 200, 200, nScale);



		if (fTimer > fFrameTime)
		{
			fTimer -= fFrameTime;
			saberposLeft.x += 200;

			nFrameCnt += 1;

			if (nFrameCnt > 4)
			{
				nFrameCnt = 0;
				animationLeft = false;
				 
				saberposLeft.x = 0;
			}
		}


	}
	else if (animationRight == true)
	{
		animationLeft = false;
		fTimer += deltatime;



		PGEptr->DrawPartialSprite(saberposRight.x, saberposRight.y, sprites[1], nFrameCnt * 200, 0, 200, 200, nScale);


		if (fTimer > fFrameTime)
		{
			fTimer -= fFrameTime;

			saberposRight.x -= 200;
			nFrameCnt += 1;

			if (nFrameCnt > 4)
			{
				nFrameCnt = 0;
				animationRight = false;
				saberposRight.x = 670;

			}
		}


	}else
	{
		fTimer += deltatime;
		
		PGEptr->DrawPartialSprite(saberposDefault.x, saberposDefault.y, sprites[3], nFrameCnt * 100, 0, 100, 100, nScale * 2);
		if (fTimer > SFrameTime)
		{
			fTimer -= SFrameTime;

			
			nFrameCnt += 1;

			if (nFrameCnt > 3)
			{
				nFrameCnt = 0;
				

			}
		}
	}
	
	
	PGEptr->SetPixelMode(olc::Pixel::NORMAL);
	
}

void Lightsaber::SpriteColorKey(olc::Pixel& fromCol, olc::Pixel& toCol, olc::Sprite* sprPtr)
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
