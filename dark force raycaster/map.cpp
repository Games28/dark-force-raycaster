#include "map.h"

bool Map::mapHasWallAt(float x, float y)
{
	if (x < 0 || x >= MAP_NUM_COLS_X * TILE_SIZE || y < 0 || y >= MAP_NUM_ROWS_Y * TILE_SIZE) {
		return true;
	}
	int mapGridIndexX = (int)(floor(x / TILE_SIZE));
	int mapGridIndexY = (int)(floor(y / TILE_SIZE));
	return map_layer_one[mapGridIndexY][mapGridIndexX] != 0;
}

bool Map::isInsideMap(float x, float y)
{
	return (x >= 0 && x <= MAP_NUM_COLS_X * TILE_SIZE && y >= 0 && y <= MAP_NUM_ROWS_Y * TILE_SIZE);
}

void Map::renderMapGrid(olc::PixelGameEngine* pge)
{
	for (int i = 0; i < MAP_NUM_ROWS_Y; i++) {
		for (int j = 0; j < MAP_NUM_COLS_X; j++) {
			int tileX = j * TILE_SIZE;
			int tileY = i * TILE_SIZE;
			olc::Pixel tileColor = map_layer_one[i][j] != 0 ? olc::BLUE : olc::WHITE;


			pge->FillRect(
				(int)(tileX * MINIMAP_SCALE_FACTOR),
				(int)(tileY * MINIMAP_SCALE_FACTOR),
				(int)(TILE_SIZE * MINIMAP_SCALE_FACTOR),
				(int)(TILE_SIZE * MINIMAP_SCALE_FACTOR),
				tileColor
			);

		}
	}
}

bool Map::test_map_has_wall_at(float x, float y, int level)
{
	if (x < 0 || x >= MAP_NUM_COLS_X * TILE_SIZE || y < 0 || y >= MAP_NUM_ROWS_Y * TILE_SIZE) {
		return true;
	}
	int mapGridIndexX = (int)(floor(x / TILE_SIZE));
	int mapGridIndexY = (int)(floor(y / TILE_SIZE));
	float mapindex = 0;
	switch (level)
	{
	case 1:
	{
		return map_layer_one[mapGridIndexY][mapGridIndexX] != 0;
	}break;

	case 2:
	{

		return map_layer_two[mapGridIndexY][mapGridIndexX] != 0;

	}break;

	case 3:
	{
		return map_layer_three[mapGridIndexY][mapGridIndexX] != 0;
	}break;

	case 4:
	{
		return map_layer_four[mapGridIndexY][mapGridIndexX] != 0;
	}break;
	}
	
}

float Map::test_get_map_at(int i, int j, int level)
{

	if (i == 14)
	{
		int u = 0;
	}
	float result;
	
	switch (level)
	{
	case 1:
	{
		result = map_layer_one[i][j];
	}break;
	case 2:
	{
		result = map_layer_two[i][j];
	}break;
	case 3:
	{
		result = map_layer_three[i][j];
	}break;
	case 4:
	{
		result = map_layer_four[i][j];
	}break;
	}
	//if(level == 1)
	//{
	//	result = map_layer_one[i][j];
	//}
	//
    //if(level == 2)
	//{
	//	result =  map_layer_two[i][j];
	//}
	//
	//if(level == 3)
	//{
	//	result = map_layer_three[i][j];
	//}
	//
	//if (level == 4)
	//{
	//	result = map_layer_four[i][j];
	//}
	//

	
	return result;
}



int Map::getMapAt(int i, int j)
{
	if (i == 14)
	{
		int u = 0;
	}
	return map_layer_one[i][j];
}


void Map::Init_lookup_tables()
{
	for (int angle = 0; angle < 360; angle++)
	{
		sin_table[angle] = sin(angle * PI / 180.0f);
		cos_table[angle] = cos(angle * PI / 180.0f);

		if (sin_table[angle] <= 0.01f && sin_table[angle] >= -0.01f)
		{
			sin_table[angle] = 0.0f;
		}

		if (cos_table[angle] <= 0.01f && cos_table[angle] >= -0.01f)
		{
			cos_table[angle] = 0.0f;
		}
	}
	int i = 0;
}

int Map::get_table_index(float angle)
{
	int result = (int)(angle * 360.0f / TWO_PI);

	return result;
}

float Map::Sin_table(int index)
{
	return sin_table[index];
}

float Map::Cos_table(int index)
{
	return cos_table[index];
}