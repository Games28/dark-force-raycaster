#define OLC_PGE_APPLICATION
#define SLICE_WIDTH 2
#define SCREEN_W 960
#define SCREEN_H 640
#define WRLD_SCALE (8 / (SLICE_WIDTH / 2) )

//test defines
#include "olcPixelGameEngine.h"
#include "textures/tatoonineTextures.ppm"
#include "textures/sky.ppm"
#include "forcepowers.h"
#include "Lightsaber.h"
#include "Player.h"
#include "Troopers.h"

//definitions
#define PI 3.1415926535f
#define P2 PI / 2.0f
#define P3 3.0f * PI / 2.0f
#define DR 0.0174533f // one degree in radians
#define MOVE_SPEED 50.0f
#define MOVE_FRACTION 0.1f
#define EPSILON 0.00001f
#define MAP_SCALE 1.0f
#define Q_PI PI * 0.25f
#define PANE_W (SCREEN_W / SLICE_WIDTH)
#define PANE_H (SCREEN_H / SLICE_WIDTH)
int glbSliceWidth = SLICE_WIDTH;
int World_Scale = WRLD_SCALE;
constexpr int mapS = 64;

float DegToRad(float a) { return a * (PI / 180.0f); }

float fixAngle(float a) { if (a >= 360.0f) { a -= 360.0f; } if (a < 0.0f) { a += 360.0f; } return a; }



typedef struct
{
	int type;
	int state;
	int map;
	bool ishit;
	float x, y, z,a;
	olc::vi2d tileSize;
	olc::vi2d curTile;
}Sprite; Sprite sprite[4], spriteHand[4];
int depth[PANE_W * PANE_H];
//int depth[SCREEN_W];


class dark_force_raycaster : public olc::PixelGameEngine
{
private:  //vaiables
	int mapX = 8, mapY = 8;
	int gameState = 0, timer = 0;
	float fade = 0;
	float offsetA;
	float fFOV = 3.14159f / 4.0f;
	bool lockon = false;
	float fobjplyA;
	olc::Sprite* spritePtrs[4] = { nullptr };
	olc::Sprite* SpriteHandPtr[3] = { nullptr };
	ForcePowers powers;
	Lightsaber saber;
	Player player;
	Troopers trooper;
	olc::vf2d oldplayerpos = { 0,0 };
	olc::vf2d newplayerpos = { 0,0 };
	float oldAngle = 0;
	float newAngle = 0;
	float fHeading = 0;
	olc::Pixel newP = olc::BLANK;
	olc::Pixel oldP = olc::MAGENTA;
	

public:  //functions

	//wall map
	int mapW[mapS] =
	{
		1,1,1,1,1,1,1,1,
		1,0,0,0,3,0,0,1,
		1,0,0,0,4,0,0,1,
		1,3,3,5,6,0,0,1,
		1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1,
	};

	//ceiling
	int mapC[mapS] =
	{
		1,1,1,1,1,0,0,0,
		1,1,1,1,1,0,0,0,
		1,1,1,1,1,0,0,0,
		1,1,1,1,1,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
	};
	//floor
	int mapF[mapS] =
	{
		2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,

	};

	void Init()
	{
		player.Init();
		player.handSprite();
		sprite[0].type = 1; sprite[0].state = 1; sprite[0].map = 0; sprite[0].x = 3.5 * 64; sprite[0].y = 6 * 64; sprite[0].z = 20; sprite[0].ishit = false; //sprite[0].a = 90 * PI / 180.0f;
		spritePtrs[0] = new olc::Sprite("textures/trooper/trooperidle.png");
		spritePtrs[1] = new olc::Sprite("textures/r2d2ground.png");
		spritePtrs[2] = new olc::Sprite("textures/newicon.png");
		spritePtrs[3] = new olc::Sprite("textures/newiconglow.png");
		for(int i = 0; i < 4;i++)
		{
		}
		trooper.Init(this);

		powers.Init();
		saber.Init();
	}

	void normalizeAngle(float& angle)
	{
		angle = remainder(angle, 2 * PI);
		if (angle < 0)
			angle = 2 * PI + angle;
	}

	void Movement(float deltaTime)
	{
		int xo, yo;
		oldplayerpos = olc::vf2d{ player.x,player.y };
		oldAngle = player.angle;
		if (player.pdx < 0) { xo = -20; }
		else { xo = 20; }
		if (player.pdy < 0) { yo = -20; }
		else { yo = 20; }

		int ipx = player.x / 64.0f, ipx_add_xo = (player.x + xo) / 64.0f, ipx_sub_xo = (player.x - xo) / 64.0f;
		int ipy = player.y / 64.0f, ipy_add_yo = (player.y + yo) / 64.0f, ipy_sub_yo = (player.y - yo) / 64.0f;

		float fSpeedDown = GetKey(olc::Key::SHIFT).bHeld ? 0.1f : 1.0f;
		float fMoveSpeed = fSpeedDown * deltaTime * 100.0f;
		float fRotSPeed = fSpeedDown * deltaTime * 2.0f;

		if (GetKey(olc::Key::W).bHeld)
		{
			if (mapW[ipy * mapX + ipx_add_xo] == 0) { player.x += player.pdx * fMoveSpeed; }
			if (mapW[ipy_add_yo * mapX + ipx] == 0) { player.y += player.pdy * fMoveSpeed; }

		}
		if (GetKey(olc::Key::S).bHeld)
		{
			if (mapW[ipy * mapX + ipx_sub_xo] == 0) { player.x -= player.pdx * fMoveSpeed; }
			if (mapW[ipy_sub_yo * mapX + ipx] == 0) { player.y -= player.pdy * fMoveSpeed; }

		}
		if (GetKey(olc::Key::A).bHeld)
		{
			player.angle += fRotSPeed; if (player.angle > 2.0f * PI) { player.angle -= 2.0f * PI; }
			player.pdx = cos(player.angle); player.pdy = -sin(player.angle);
		}
		if (GetKey(olc::Key::D).bHeld)
		{
			player.angle -= fRotSPeed; if (player.angle < 0) { player.angle += 2.0f * PI; }
			player.pdx = cos(player.angle); player.pdy = -sin(player.angle);
		}
		if (GetKey(olc::Key::E).bHeld)
		{
			//if (mapW[ipy * mapX + ipx_add_xo] == 0) { player.y += cos(player.angle) * fMoveSpeed; }
			//if (mapW[ipy_add_yo * mapX + ipx] == 0) { player.x -= sin(player.angle) * fMoveSpeed; }
			player.y += player.pdx * fMoveSpeed;
			player.x -= player.pdy * fMoveSpeed;
		}
		if (GetKey(olc::Key::Q).bHeld)
		{
			//if (mapW[ipy * mapX + ipx_add_xo] == 0) { player.y -= cos(player.angle) * fMoveSpeed; }
			//if (mapW[ipy_add_yo * mapX + ipx] == 0) { player.x += sin(player.angle) * fMoveSpeed; }
			player.y -= player.pdx * fMoveSpeed;
			player.x += player.pdy * fMoveSpeed;
		}
		newplayerpos = olc::vf2d{ player.x,player.y };
		newAngle = player.angle;
	}

	
	//draw functions

	void drawPlayer()
	{
		FillRect((player.x - 4) * MAP_SCALE, (player.y - 4) * MAP_SCALE, 8 * MAP_SCALE, 8 * MAP_SCALE, olc::YELLOW);
		DrawLine(player.x * MAP_SCALE, player.y * MAP_SCALE,
			(player.x + player.pdx * 10.0f) * MAP_SCALE, (player.y + player.pdy * 10.0f) * MAP_SCALE, olc::YELLOW);
	}

	void drawMap()
	{
		int x, y;
		for (y = 0; y < mapY; y++)
		{
			for (x = 0; x < mapX; x++)
			{
				int worldX = x * mapS;
				int worldY = y * mapS;
				if (mapW[y * mapX + x] > 0)
				{
					FillRect(worldX * MAP_SCALE, worldY * MAP_SCALE, mapS * MAP_SCALE, mapS * MAP_SCALE, olc::WHITE);
					DrawRect(worldX * MAP_SCALE, worldY * MAP_SCALE, mapS * MAP_SCALE, mapS * MAP_SCALE, olc::BLACK);
				}
				else
				{
					FillRect(worldX * MAP_SCALE, worldY * MAP_SCALE, mapS * MAP_SCALE, mapS * MAP_SCALE, olc::BLACK);
					DrawRect(worldX * MAP_SCALE, worldY * MAP_SCALE, mapS * MAP_SCALE, mapS * MAP_SCALE, olc::WHITE);
				}
			}
		}
	}

	void drawRays()
	{
		int r, mx, my, mp, dof, side; float vx, vy, rx, ry, ra, xo, yo, disV, disH;
		ra = player.angle + 30.0f * DR;
		normalizeAngle(ra);

		int nNrSlices = SCREEN_W / glbSliceWidth;
		for (r = 0; r < nNrSlices; r++)
		{
			int vmt = 0, hmt = 0;
			//---Vertical---
			dof = 0; side = 0; disV = 100000;
			float Tan = tan(ra);
			if (cos(ra) > 0.001) { rx = (((int)player.x >> 6) << 6) + 64;      ry = (player.x - rx) * Tan + player.y; xo = 64; yo = -xo * Tan; }//looking left
			else if (cos(ra) < -0.001) { rx = (((int)player.x >> 6) << 6) - 0.0001; ry = (player.x - rx) * Tan + player.y; xo = -64; yo = -xo * Tan; }//looking right
			else { rx = player.x; ry = player.y; dof = 8; }                                                  //looking up or down. no hit

			while (dof < 8)
			{
				mx = (int)(rx) >> 6; my = (int)(ry) >> 6; mp = my * mapX + mx;
				if (mp > 0 && mp < mapX * mapY && mapW[mp] > 0) { vmt = mapW[mp] - 1; dof = 8; disV = cos(ra) * (rx - player.x) - sin(ra) * (ry - player.y); }//hit
				else { rx += xo; ry += yo; dof += 1; }                                               //check next horizontal
			}
			vx = rx; vy = ry;


			//DrawLine(player.x * MAP_SCALE, player.y * MAP_SCALE, rx * MAP_SCALE, ry * MAP_SCALE, olc::DARK_GREEN);

			//---Horizontal---

			dof = 0; disH = 100000;
			Tan = 1.0 / Tan;
			if (sin(ra) > 0.001) { ry = (((int)player.y >> 6) << 6) - 0.0001; rx = (player.y - ry) * Tan + player.x; yo = -64; xo = -yo * Tan; }//looking up
			else if (sin(ra) < -0.001) { ry = (((int)player.y >> 6) << 6) + 64; rx = (player.y - ry) * Tan + player.x; yo = 64; xo = -yo * Tan; }//looking down
			else { rx = player.x; ry = player.y; dof = 8; }                                                   //looking straight left or right

			while (dof < 8)
			{
				mx = (int)(rx) >> 6; my = (int)(ry) >> 6; mp = my * mapX + mx;
				if (mp > 0 && mp < mapX * mapY && mapW[mp] > 0) { hmt = mapW[mp] - 1; dof = 8; disH = cos(ra) * (rx - player.x) - sin(ra) * (ry - player.y); }//hit
				else { rx += xo; ry += yo; dof += 1; }                                               //check next horizontal
			}

			float shade = 1;
			olc::Pixel p = olc::GREEN;
			if (disV < disH) { hmt = vmt; shade = 0.5; rx = vx; ry = vy; disH = disV;  p = olc::GREEN * 0.5f; }
			//DrawLine(player.x * MAP_SCALE, player.y * MAP_SCALE, rx * MAP_SCALE, ry * MAP_SCALE, p);

			drawWall(r, ra, disH, rx, ry, shade, hmt);
//			ra -= 0.5f * DR;       // Joseph21 - ra is in radians, and FixAng works for degrees...
			ra -= DR * (60.0f / nNrSlices);       // Joseph21 - ra is in radians, and FixAng works for degrees...
			normalizeAngle(ra);
		}
	}

	void drawWall(int r, float ra, float disH, float rx, float ry, float shade, int hmt)
	{
		float ca = player.angle - ra;
		disH = disH * cos(ca);
		float lineH = (mapS * 640) / disH;
		float ty_step = 32.0f / lineH;
		float ty_off = 0;
		if (lineH > 640) { ty_off = (lineH - 640) / 2.0f; lineH = 640; }
		int lineoff = 320 - (lineH / 2);
		depth[r] = disH;

		int y;
		float ty = ty_off * ty_step;
		float tx;
		if (shade == 1)
		{
			tx = (int)(rx / 2.0f) % 32; if (ra > 180) { tx = 31 - tx; }
		}
		else {
			tx = (int)(ry / 2.0) % 32; if (ra > 90 && ra < 270) { tx = 31 - tx; }
		}

		for (y = 0; y < lineH; y++)
		{
			int pixel = ((int)ty * 32 + (int)tx) * 3 + (hmt * 32 * 32 * 3);
			int red = tatoonineTextures[pixel + 0] * shade;
			int green = tatoonineTextures[pixel + 1] * shade;
			int blue = tatoonineTextures[pixel + 2] * shade;

			FillRect(r * SLICE_WIDTH, lineoff + y, SLICE_WIDTH, 1, olc::Pixel(red, green, blue));

			ty += ty_step;

		}

		//---draw floors---
		for (y = lineoff + lineH; y < 640; y++)
		{

			float dy = y - (640 / 2.0), deg = ra, raFix = cos(player.angle - ra);       // Joseph21 - pa and ra are in radians already

			tx = player.x / 2 + cos(deg) * 158 * 2 * 32 / dy / raFix;
			ty = player.y / 2 - sin(deg) * 158 * 2 * 32 / dy / raFix;
			int nmp = mapF[(int)(ty / 32.0) * mapX + (int)(tx / 32.0)] * 32 * 32;
			int pixel = (((int)ty & 31) * 32 + ((int)tx & 31)) * 3 + nmp * 3;
			int red = tatoonineTextures[pixel + 0] * 0.7;
			int green = tatoonineTextures[pixel + 1] * 0.7;
			int blue = tatoonineTextures[pixel + 2] * 0.7;



			FillRect(r * SLICE_WIDTH, y, SLICE_WIDTH, 1, olc::Pixel(red, green, blue));

			//---draw ceiling---
			nmp = mapC[(int)(ty / 32.0) * mapX + (int)(tx / 32.0)] * 32 * 32;
			pixel = (((int)ty & 31) * 32 + ((int)tx & 31)) * 3 + nmp * 3;
			red = tatoonineTextures[pixel + 0];
			green = tatoonineTextures[pixel + 1];
			blue = tatoonineTextures[pixel + 2];


			if (nmp > 0) {
				FillRect(r * SLICE_WIDTH, 640 - y, SLICE_WIDTH, 1, olc::Pixel(red, green, blue));
			}
		}
	}

	void drawSky()
	{
		int nNrColumns = SCREEN_W / glbSliceWidth;
		int nNrRows    = SCREEN_H / glbSliceWidth;

        float fScaleX = (SCREEN_W / 120) / float(glbSliceWidth);
        float fScaleY = (SCREEN_H / 160) / float(glbSliceWidth);

		for (int y = 0; y < nNrRows; y++)
		{
			for (int x = 0; x < nNrColumns; x++)
			{
				int xo = int(player.angle / DR) * 2 - (x / fScaleX);   // Joseph21 - here we need degree value so divide pa by DR
				if (xo < 0) { xo += 120; }
				xo = xo % 120;

				int yo = y / fScaleY;

				int pixel = (yo * 120 + xo) * 3;
				int red   = sky[pixel + 0];
				int green = sky[pixel + 1];
				int blue  = sky[pixel + 2];

				FillRect(x * SLICE_WIDTH, y * SLICE_WIDTH, SLICE_WIDTH, SLICE_WIDTH, olc::Pixel(red, green, blue));
			}
		}
	}

	

	void NewDraw()
	{
		auto is_between_pi_factors = [=](float fAngle, float fFactorLow, float fFactorHigh) {
            // Joseph21 - if you have PI defined as a constant, then use PI iso 3.14159f
			return ((fAngle >= PI * fFactorLow) && (fAngle < PI * fFactorHigh));
        };

		float sx = sprite[0].x - player.x;
		float sy = sprite[0].y - player.y;
		float sz = sprite[0].z;
		float x, y;
		float fEyeX = cosf(player.angle);
		float fEyeY = sinf(player.angle);
		int nTextureSize = 100;
		olc::vi2d curTile = { 0,0 };
		olc::vi2d tileSize = { 100,100 };

		float  fObjectAngle = -atan2f(sy, sx) - atan2f(fEyeY, fEyeX);
		fHeading = player.angle - sprite[0].a + 3.14159f / 4.0f;
		if (fHeading < 0) fHeading += 2.0f * 3.14159f;
		if (fHeading >= 2.0f * 3.14159f) fHeading -= 2.0f * 3.14159f;
//		
		if (fObjectAngle < -PI) fObjectAngle += 2.0f * PI;
		if (fObjectAngle >  PI) fObjectAngle -= 2.0f * PI;

		bool bInPlayerFOV = fabs(fObjectAngle) < fFOV / 2.0f;

		float fDistanceFromPlayer = sqrtf(sx * sx + sy * sy);

		int CellSize   = 64;
		int sliceWidth =  0;
		int panewidth  =  0;
		int paneheight =  0;

		float fObjectCeiling     = 0;
		float fObjectFloor       = 0;
		float fObjectHeight      = 0;
		float fObjectAspectRatio = 0;
		float fObjectWidth       = 0;
		float fMiddleOfObject    = 0;

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
			sliceWidth = SLICE_WIDTH;    // Joseph21 - please don't use hardcoded constants if you have defined symbolic ones
                                         //            suggestion: you could use only the constant and remove the variable
			panewidth  = ScreenWidth()  / sliceWidth;
			paneheight = ScreenHeight() / sliceWidth;

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
                       float fSpriteFraction = 1.0f / float( nSubSpritesX );
                       //// Joseph21 - determine sample coordinate depending on selected sub sprite and number of subsprites in sprite file
                      float fSampleX = (x / fObjectWidth ) * fSpriteFraction + nSelectedSubSprite * fSpriteFraction;
                      float fSampleY =  y / fObjectHeight;
   					   int nObjectColumn = (int)(fMiddleOfObject + x - (fObjectWidth / 2.0f));

					   
                       olc::Pixel samplePixel = spritePtrs[1]->Sample(fSampleX,  fSampleY);
					   
                        if (samplePixel != olc::MAGENTA && nObjectColumn >= 0 && nObjectColumn < panewidth  && depth[nObjectColumn] >= fDistanceFromPlayer)
                        {
   					   	  
                           // Joseph21 - use nObjectColumn to determine screen location for this pixel
                           FillRect(nObjectColumn * sliceWidth, (fObjectCeiling + y) * sliceWidth, sliceWidth, sliceWidth, samplePixel);
					   
                           // Joseph21 - distance checking is moved outside this column loop
						   depth[nObjectColumn] = fDistanceFromPlayer;
                        }
						
                    }
					
					
                }
				
			}
		}

		    // Joseph21 - fixed typo
		
	}


	bool isInSight(float objectx, float objecty, float fov, float &angle2player)
	{
		// little lambda to ensure result is in [0, 2 * pi )
		auto ModuloTwoPi = [=](float angle) {
			float a = angle;
			while (a < 0) a += 2.0f * 3.14159f;
			while (a >= 2.0f * 3.14159f) a -= 2.0f * 3.14159f;
			return a;
		};

		// this is for easy adressing of player characteristics
		//LevelType& cl = glbLevel[nCurLevel];
		// work out object coordinates when translated so that player is at origin
		float Tempx = objectx - player.x;
		float Tempy = objecty - player.y;
		// determine angle from object to player - the minus is needed since the y value on the screen is reversed wrt regular math
		angle2player = ModuloTwoPi(atan2(-Tempy, Tempx));
		// compare the two angles and return if they are within a specified range
		return (abs(ModuloTwoPi(player.angle) - angle2player) < fov) ||
			(abs(ModuloTwoPi(player.angle) - angle2player) > (2.0f * PI - fov));
	}

	void SpriteColorKey(olc::Pixel& fromCol, olc::Pixel& toCol, olc::Sprite* sprPtr)
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

public: //main function


	dark_force_raycaster()
	{
		sAppName = "Dark Force Rayaster";
	}

	bool OnUserCreate() override
	{
		Init();


		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLACK);
		float fnotuse = -1.0f;
		Movement(fElapsedTime);
		drawSky();
		//drawMap();
		//drawPlayer();
		drawRays();
		//drawSpriteTest();
		trooper.Draw(this, player.x, player.y, player.angle);
		NewDraw();
		if (GetKey(olc::Key::SPACE).bHeld)
		{
			powers.Draw(this);
		}

		
		player.Draw(this);
		saber.Draw(this, fElapsedTime,player.x,player.y);
		lockon = isInSight(sprite[0].x, sprite[0].y, 20.0f * PI / 180.0f, fnotuse);
		if (GetKey(olc::Key::V).bHeld && lockon == true)
		{
			sprite[0].z = 0;
			powers.tkRotation(sprite[0].x, sprite[0].y, player.x, player.y, oldAngle, newAngle);
			powers.tkMove(sprite[0].x, sprite[0].y, oldplayerpos.x, oldplayerpos.y, newplayerpos.x, newplayerpos.y);
		}
		else {
			sprite[0].z = 20;
		}

		if (isInSight(sprite[0].x, sprite[0].y, 10.0f * PI / 180.0f, fnotuse) == true)
		{
			DrawSprite(400, 80, spritePtrs[3]);
		}
		else {
			DrawSprite(400, 80, spritePtrs[2]);
		}

		return true;
	}


};


int main()
{
	dark_force_raycaster game;
	if (game.Construct(960, 640, 1, 1))
		game.Start();

	return 0;

}