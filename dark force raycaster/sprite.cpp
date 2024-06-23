#include "sprite.h"

void Sprite::Init_Sprites(Map* mapPtr)
{
	for (int i = 0; i < spritetexturefilenames.size(); i++)
	{
		olc::Sprite* newsprite = new olc::Sprite(spritetexturefilenames[i]);
		spritetextures.push_back(newsprite);
	}

	sprites[0] = { 300, 400, 2,PI / 2 };
}

void Sprite::renderSpriteProjection(olc::PixelGameEngine* pge, Player& player, Rays& ray)
{
	sprite_t visibleSprites[NUM_SPRITES];
	int numVisibleSprites = 0;

	for (int i = 0; i < NUM_SPRITES; i++) {
		float angleSpritePlayer = player.rotationAngle - atan2(sprites[i].y - player.y, sprites[i].x - player.x);

		if (angleSpritePlayer > PI)
			angleSpritePlayer -= TWO_PI;
		if (angleSpritePlayer < -PI)
			angleSpritePlayer += TWO_PI;

		angleSpritePlayer = fabs(angleSpritePlayer);

		const float EPSILON = 0.2f;
		if (angleSpritePlayer < (FOV_ANGLE / 2) + EPSILON) {
			sprites[i].visible = true;
			sprites[i].rotationAngle = angleSpritePlayer;
			sprites[i].distance = distanceBetweenPoints(sprites[i].x, sprites[i].y, player.x, player.y);
			visibleSprites[numVisibleSprites] = sprites[i];
			numVisibleSprites++;
		}
		else {
			sprites[i].visible = false;
		}
	}

	for (int i = 0; i < numVisibleSprites - 1; i++)
	{
		for (int j = i + 1; j < numVisibleSprites; j++)
		{
			if (visibleSprites[i].distance < visibleSprites[j].distance)
			{
				sprite_t temp = visibleSprites[i];
				visibleSprites[i] = visibleSprites[j];
				visibleSprites[j] = temp;
			}
		}
	}

	for (int i = 0; i < numVisibleSprites; i++)
	{
		sprite_t sprite = visibleSprites[i];
		//sprite.movement;

		float prepDistance = sprite.distance * cos(sprite.rotationAngle);

		float spriteHeight = (TILE_SIZE / prepDistance) * Dist_PROJ_PLANE;
		float spriteWidth = spriteHeight;

		float spriteTopY = (WINDOW_HEIGHT / 2) - (spriteHeight / 2);
		spriteTopY = (spriteTopY < 0) ? 0 : spriteTopY;

		float spriteBottomY = (WINDOW_HEIGHT / 2) + (spriteHeight / 2);
		spriteBottomY = (spriteBottomY > WINDOW_HEIGHT) ? WINDOW_HEIGHT : spriteBottomY;

		float spriteAngle = atan2(sprite.y - player.y, sprite.x - player.x) - player.rotationAngle;
		float spriteScreenPosX = tan(spriteAngle) * Dist_PROJ_PLANE;

		float spriteLeftX = (WINDOW_WIDTH / 2) + spriteScreenPosX - spriteWidth / 2;
		float spriteRightX = spriteLeftX + spriteWidth;

		int textureWidth = spritetextures[sprite.texture]->width;
		int textureHeight = spritetextures[sprite.texture]->height;

		for (int x = (int)spriteLeftX; x < (int)spriteRightX; x++)
		{

			float texelWidth = (textureWidth / spriteWidth);
			int textureOffSetX = (x - spriteLeftX) * texelWidth;
			for (int y = (int)spriteTopY; y < (int)spriteBottomY; y++)
			{
				int distanceFromTop = y + (spriteHeight / 2) - (WINDOW_HEIGHT / 2);
				int textureOffSetY = distanceFromTop * (textureHeight / spriteHeight);

				if (x > 0 && x < WINDOW_WIDTH && y > 0 && y < WINDOW_HEIGHT)
				{

					//color_t* spriteTextureBuffer = (color_t*)upng_get_buffer(texture.textures[sprite.texture]);
					//color_t texelColor = spriteTextureBuffer[(textureWidth * textureOffSetY) + textureOffSetX];
					olc::Pixel texelColor = spritetextures[sprite.texture]->GetPixel(textureOffSetX, textureOffSetY);
					if (sprite.distance < ray.rays[x].HitListType[0].front_distance && texelColor != 0xffff00ff)
						pge->Draw(x, y, texelColor);
				}

			}
		}

	}
}

void Sprite::renderMapSprites(olc::PixelGameEngine* pge)
{
	for (int i = 0; i < NUM_SPRITES; i++)
	{
		for (int i = 0; i < NUM_SPRITES; i++) {
			pge->FillRect(
				(int)(sprites[i].x * MINIMAP_SCALE_FACTOR),
				(int)(sprites[i].y * MINIMAP_SCALE_FACTOR),
				4,
				4,
				(sprites[i].visible) ? olc::GREY : olc::DARK_GREY
			);
		}
	}
	for (int i = 0; i < NUM_SPRITES; i++)
	{
		sprite_t sprite = sprites[i];
		pge->DrawLine(
			(int)(sprite.x * MINIMAP_SCALE_FACTOR),
			(int)(sprite.y * MINIMAP_SCALE_FACTOR),
			(int)((sprite.x + cos(sprite.rotationAngle) * 40) * MINIMAP_SCALE_FACTOR),
			(int)((sprite.y + sin(sprite.rotationAngle) * 40) * MINIMAP_SCALE_FACTOR),
			olc::BLUE);
	}
}

float Sprite::distanceBetweenPoints(float x1, float y1, float x2, float y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void Sprite::normalizeAngle(float* angle)
{
	*angle = remainder(*angle, TWO_PI);
	if (*angle < 0) {
		*angle = TWO_PI + *angle;
	}

}
