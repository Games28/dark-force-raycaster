#include "olcPixelGameEngine.h"


class ForcePowers 
{
public:
	//ForcePowers(olc::PixelGameEngine* PGEptr);
	ForcePowers() = default;
	void Init();
	void Draw(olc::PixelGameEngine* PGEptr);
	void DrawLightning(olc::PixelGameEngine* PGEptr,int x1, int y1, int x2, int y2, int depth, olc::Pixel p);
	void tkRotation(float &objectx, float &objecty, float playerx, float playery, float oldAngle, float newAngle);
	void tkMove(float &objectx, float &objecty, float oldplayerx, float oldplayery, float newplayerx, float nowplayery);

private:
	int screenW = 940 / 2;
	int screenH = 640 / 2;
	olc::PixelGameEngine* PGEptr = nullptr;
	olc::vf2d LightningLeftHand[5];
	olc::vf2d LightningRightHand[5];
	olc::vf2d LightningDestination = { screenW,screenH };
	olc::vi2d HandPos = { 100, 350 };
	float HandX = 100;
	float HandY = 350;
	
};