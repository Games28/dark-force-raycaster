#include "wall.h"

void Wall::Init_texture()
{
	

	for (int i = 0; i < walltexturefilenames.size(); i++)
	{
		olc::Sprite* newsprite = new olc::Sprite(walltexturefilenames[i]);
		walltextures.push_back(newsprite);
	}

	isTranparent =
	{
		false, // 0
		false, // 1
		false, // 2
		false, // 3
		false, // 4
		false, // 5
		false, // 6
		true,  // 7 istransparent
		true   // 8 istransparent
	};

}



void Wall::CalculateWallBottomAndTop2(float fCorrectedDistToWall, int nHorHeight, int nLevelHeight, float fWallHeight, int& nWallTop, int& nWallBottom, Player& player)
{
    int nSliceHeight = int((TILE_SIZE / fCorrectedDistToWall) * Dist_PROJ_PLANE);
    nWallTop = nHorHeight - (float(nSliceHeight) * (1.0f - player.fPlayerH)) - (nLevelHeight + fWallHeight - 1) * nSliceHeight;
    nWallBottom = nWallTop + nSliceHeight * fWallHeight;
}

void Wall::changeColorIntensity(olc::Pixel* color, float factor)
{
	olc::Pixel p;
	p.r = color->r * factor;
	p.g = color->g * factor;
	p.b = color->b * factor;

	*color = p;
}

void Wall::renderWallProjection(olc::PixelGameEngine* pge, Rays& ray, Player& player)
{
    int nHorizonHeight = WINDOW_HEIGHT * player.fPlayerH;

    for (int x = 0; x < NUM_RAYS; x++) {

         int nX_hit, nY_hit, nWallTop1, nWallTop2, nWallBottom1, nWallBottom2, nBlockLevel;
            float fX_hit, fY_hit, fBlockElevation, fFrontDistance, fBackDistance;
            


        std::vector<DelayedPixel> vDelayedPixels;

        bool bFinished = false;

        float fFoV = 60.0f;
        float fAngleStep = fFoV / (float)NUM_RAYS;
        float fViewAngle = (float)(x - (NUM_RAYS / 2)) * fAngleStep;
        float fCurAngle = (player.rotationAngle * 180.0f / PI) + fViewAngle;
        float fPlayerX = player.x;
        float fPlayerY = player.y;
        float fCosCurAngle = cos(fCurAngle * PI / 180.0f);
        float fSinCurAngle = sin(fCurAngle * PI / 180.0f);
        float fCosViewAngle = cos(fViewAngle * PI / 180.0f);

        auto get_ceiling_sample = [=](int px, int py, float fHeight) -> olc::Pixel {
            float fFloorProjDistance = (((TILE_SIZE * player.fPlayerH) / (float)(nHorizonHeight - py)) * Dist_PROJ_PLANE) / fCosViewAngle;
            float fFloorProjX = fPlayerX + fFloorProjDistance * fCosCurAngle;
            float fFloorProjY = fPlayerY + fFloorProjDistance * fSinCurAngle;
            int nSampleX = (int)(fFloorProjX) % TILE_SIZE;
            int nSampleY = (int)(fFloorProjY) % TILE_SIZE;

            //olc::Pixel color = olc::BLUE;
            olc::Pixel color = walltextures[4]->GetPixel(nSampleX, nSampleY);
            return color;

        };

        auto get_floor_sample = [=](int px, int py, float fHeight) -> olc::Pixel {
            float fFloorProjDistance = (((TILE_SIZE * player.fPlayerH) / (float)(py - nHorizonHeight)) * Dist_PROJ_PLANE) / fCosViewAngle;
            float fFloorProjX = fPlayerX + fFloorProjDistance * fCosCurAngle;
            float fFloorProjY = fPlayerY + fFloorProjDistance * fSinCurAngle;
            
           
            int nSampleX = (int)(fFloorProjX) % TILE_SIZE;
            int nSampleY = (int)(fFloorProjY) % TILE_SIZE;
            //olc::Pixel p = olc::GREEN;
            olc::Pixel p = walltextures[0]->GetPixel(nSampleX, nSampleY);
            return p;
           

        };
        auto get_roof_sample = [=](int px, int py, float fHeight) -> olc::Pixel {
            float fFloorProjDistance = ((((TILE_SIZE * player.fPlayerH) - fHeight) / (float)(py - nHorizonHeight)) * Dist_PROJ_PLANE) / fCosViewAngle;
            float fFloorProjX = fPlayerX + fFloorProjDistance * fCosCurAngle;
            float fFloorProjY = fPlayerY + fFloorProjDistance * fSinCurAngle;
            
            int nSampleX = (int)(fFloorProjX) % TILE_SIZE;
            int nSampleY = (int)(fFloorProjY) % TILE_SIZE;
            olc::Pixel p = olc::GREY;
            //olc::Pixel p = walltextures[0]->GetPixel(nSampleX, nSampleY);
            return p;

        };

       

       
            for (int i = 0; i < (int)ray.rays[x].size(); i++)
            {
                float prepDistance = ray.rays[x][i].front_distance * cos(ray.rays[x][i].rayAngle - player.rotationAngle);
                //ceiling_front and front_bottom
               
                CalculateWallBottomAndTop2(prepDistance, nHorizonHeight, ray.rays[x][i].maplayer, ray.rays[x][i].fHeight, ray.rays[x][i].top_front, ray.rays[x][i].bottom_front, player);
            }

            for (int i = 0; i < (int)ray.rays[x].size(); i++)
            {
                if (i == (int)ray.rays[x].size() - 1)
                {
                    ray.rays[x][i].top_back = ray.rays[x][i].top_front;
                    ray.rays[x][i].Bottom_back = ray.rays[x][i].bottom_front;
                    ray.rays[x][i].Back_distance = ray.rays[x][i].front_distance;
                }
                else
                {
                    ray.rays[x][i].Back_distance = ray.rays[x][i + 1].front_distance;
                    CalculateWallBottomAndTop2(ray.rays[x][i].Back_distance, nHorizonHeight, ray.rays[x][i].maplayer, ray.rays[x][i].fHeight, ray.rays[x][i].top_back, ray.rays[x][i].Bottom_back, player);
                }
            }



        for (int y = WINDOW_HEIGHT - 1; y >= 0; y--)
        {
            player.fDepthBuffer[y * WINDOW_WIDTH + x] = player.fmaxDist;

            if (y < nHorizonHeight)
            {
                //olc::Pixel rooksample = get_ceiling_sample(x, y, 1);
                pge->Draw(x, y, olc::CYAN);
                //player.DrawDepth(pge, player.fmaxDist, x, y, olc::CYAN);
            }
            else
            {
               olc::Pixel floorSample = get_floor_sample(x, y,1);
               //player.DrawDepth(pge, player.fmaxDist, x, y, floorSample);
                pge->Draw(x, y, floorSample);

            }

        }

        for (int i = 0; i < (int)ray.rays[x].size(); i++)
        {
            if (ray.rays[x][i].fHeight > 0.0f)
            {
                fX_hit = ray.rays[x][i].wallHitX;
                fY_hit = ray.rays[x][i].wallHitY;
                nX_hit = (int)ray.rays[x][i].wallHitX / TILE_SIZE;
                nY_hit = (int)ray.rays[x][i].wallHitY / TILE_SIZE;
                fBlockElevation = ray.rays[x][i].fHeight;
                nBlockLevel = ray.rays[x][i].maplayer;
                fFrontDistance = ray.rays[x][i].front_distance;
                fBackDistance = ray.rays[x][i].Back_distance;

                nWallTop1 = std::clamp(ray.rays[x][i].top_front, 0, WINDOW_HEIGHT - 1);
                nWallTop2 = std::clamp(ray.rays[x][i].top_back, 0, WINDOW_HEIGHT - 1);
                nWallBottom1 = std::clamp(ray.rays[x][i].bottom_front, 0, WINDOW_HEIGHT - 1);
                nWallBottom2 = std::clamp(ray.rays[x][i].Bottom_back, 0, WINDOW_HEIGHT - 1);
                

                //render roof needs testing
                for (int y = nWallTop2; y < nWallTop1; y++)
                {
                    olc::Pixel roofSample = get_roof_sample(x, y, float(nBlockLevel) + fBlockElevation);
                    //player.DrawDepth(pge, fBackDistance, x, y, roofSample);
                    pge->Draw(x, y, olc::CYAN);
                }

                //render wall segment
                bool bSampleX_known = false;
                float fSampleX;
               

               
                for (int y = nWallTop1; y <= nWallBottom1; y++)
                {
                   
                    //float fSampleY = (float)( y - nWallTop1) / (nWallBottom1 - nWallTop1);
                  

                        if (ray.rays[x][i].wasHitVertical) {
                            fSampleX = (int)ray.rays[x][i].wallHitY % TILE_SIZE;
                        }
                        else
                        {
                            fSampleX = (int)ray.rays[x][i].wallHitX % TILE_SIZE;
                        }
                    
                    float fSampleY = fBlockElevation * float(y - ray.rays[x][i].top_front) / float(ray.rays[x][i].bottom_front - ray.rays[x][i].top_front);
                    fSampleY = fSampleY * float(TILE_SIZE);

                    //working
                    //pge->Draw(x, y, olc::BLUE);

                    olc::Pixel wallSample = walltextures[1]->GetPixel((int)fSampleX, (int)fSampleY);
                    //player.DrawDepth(pge, fFrontDistance, x, y, wallSample);
                    pge->Draw(x, y, wallSample);
                }

                for (int y = nWallBottom1; y <= nWallBottom2; y++)
                {
                    olc::Pixel CeilSample = get_ceiling_sample(x, y, float(nBlockLevel) + fBlockElevation);

                    //player.DrawDepth(pge, fBackDistance, x, y, CeilSample);
                    pge->Draw(x, y, olc::GREEN);
                }

                if (x == 300 && i == (int)ray.rays[x].size() - 2)
                {

                    pge->DrawLine(x,nWallBottom1, x, nWallTop1, olc::YELLOW);
                   // pge->DrawString(200, 30, "maplayer: " + std::to_string(ray.rays[x][i].maplayer) + " blockheight: " + std::to_string(ray.rays[x][i].fHeight));
                }
                
            }

        }
    }
}

