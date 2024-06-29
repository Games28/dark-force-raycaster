#include "application.h"





void Application::Setup()
{
	player.Init(&map);
	wall.Init_texture();
	sprite.Init_Sprites(&map);
	map.Init_lookup_tables();
	player.fmaxDist = player.fMaxDistance();
}

void Application::ProcessInput(olc::PixelGameEngine* pge)
{
	if (pge->GetKey(olc::W).bHeld) player.walkDirection = 1;
	if (pge->GetKey(olc::S).bHeld) player.walkDirection = -1;
	if (pge->GetKey(olc::A).bHeld) player.turnDirection = -1;
	if (pge->GetKey(olc::D).bHeld) player.turnDirection = 1;
	if (pge->GetKey(olc::Q).bHeld) player.strafeDirection = 1;
	if (pge->GetKey(olc::E).bHeld) player.strafeDirection = -1;

	if (pge->GetKey(olc::W).bReleased) player.walkDirection = 0;
	if (pge->GetKey(olc::S).bReleased) player.walkDirection = 0;
	if (pge->GetKey(olc::A).bReleased) player.turnDirection = 0;
	if (pge->GetKey(olc::D).bReleased) player.turnDirection = 0;
	if (pge->GetKey(olc::Q).bReleased) player.strafeDirection = 0;
	if (pge->GetKey(olc::E).bReleased) player.strafeDirection = 0;
}

void Application::Update(float Deltatime)
{
	player.move(Deltatime);
	float ix = map.cos_table[10];
	float iy = map.sin_table[10];
	player.strafe(Deltatime);
	ray.castAllRays(player, map);
	
}

void Application::Render(olc::PixelGameEngine* pge)
{
	wall.renderWallProjection(pge,ray,player);
	sprite.renderSpriteProjection(pge, player, ray);
	map.renderMapGrid(pge);
	ray.renderMapRays(pge, player);
	player.render(pge);
	sprite.renderMapSprites(pge);


}
