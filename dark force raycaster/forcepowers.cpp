#include "forcepowers.h"




void ForcePowers::Init()
{
	LightningLeftHand[0] = olc::vf2d{HandX + 112,HandY + 120 }; //pinky
	LightningLeftHand[1] = olc::vf2d{HandX + 152,HandY + 80 }; // ring
	LightningLeftHand[2] = olc::vf2d{HandX + 212,HandY + 64 }; // middle
	LightningLeftHand[3] = olc::vf2d{HandX + 260,HandY + 97 }; // pointer
	LightningLeftHand[4] = olc::vf2d{HandX + 280,HandY + 224 }; // thumb
}

void ForcePowers::Draw(olc::PixelGameEngine* PGEptr)
{
	olc::Pixel LightningColor = olc::DARK_BLUE;
	int depth = 4;
	for (int i = 0; i < 5; i++)
	{
		//left hand
		DrawLightning(PGEptr,LightningLeftHand[i].x,LightningLeftHand[1].y, LightningDestination.x,LightningDestination.y,depth, LightningColor);
	}
}

void ForcePowers::DrawLightning(olc::PixelGameEngine* PGEptr,int x1, int y1, int x2, int y2, int depth, olc::Pixel p)
{
	auto RandAroundZero = [=]() {
		return ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
	};

	if (depth == 0)
	{
		PGEptr->DrawLine(x1, y1, x2, y2, p);
	}
	else if (depth > 0)
	{
		int nDepthX = x2 - x1;
		int nDepthY = y2 - y1;

		float fPercentage = 0.30f;
		float fMinimum = 2.50f;

		float fMarginX = std::max(fMinimum, fPercentage * (float)nDepthX);
		float fMarginY = std::max(fMinimum, fPercentage * (float)nDepthY);

		int x_mid = x1 + nDepthX / 2 + (int)(RandAroundZero() * fMarginX);
		int y_mid = y1 + nDepthY / 2 + (int)(RandAroundZero() * fMarginY);

		DrawLightning( PGEptr,x1, y1, x_mid, y_mid, depth - 1, p);
		DrawLightning(PGEptr, x_mid, y_mid, x2, y2, depth - 1, p);
	}

}

void ForcePowers::tkRotation(float &objectx, float &objecty, float playerx, float playery, float oldAngle, float newAngle)
{
	float difference_x = objectx - playerx;
	float difference_y = objecty - playery;

	float distance = sqrtf(difference_x * difference_x + difference_y * difference_y);
	float angle_player_to_object = atan2f(difference_y, difference_x);
	
	float angle_difference = oldAngle - newAngle;
	
	//angle_difference *= dir;
	objectx += distance * (cosf(angle_player_to_object + angle_difference) - cosf(angle_player_to_object));
	objecty += distance * (sinf(angle_player_to_object + angle_difference) - sinf(angle_player_to_object));
}

void ForcePowers::tkMove(float &objectx, float &objecty, float oldplayerx, float oldplayery, float newplayerx, float newplayery)
{
	float differencex = newplayerx - oldplayerx;
	float differencey = newplayery - oldplayery;
	objectx += differencex;
	objecty += differencey;

}
