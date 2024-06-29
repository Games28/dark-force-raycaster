#ifndef MAP_H
#define MAP_H
#include "olcPixelGameEngine.h"
#include "vector.h"
#include "defs.h"
#include "mapeditor.h"


class Map
{
public:
	Map() = default;

    //working stuff
	bool mapHasWallAt(float x, float y);
	bool isInsideMap(float x, float y);
    int getMapAt(int i, int j);

	void renderMapGrid(olc::PixelGameEngine* pge);
	

    //test functions for multiple layers
    bool test_map_has_wall_at(float x, float y, int level);
    float test_get_map_at(int i, int j, int level);

private:
   

public:

    //lookup tables
    float sin_table[360];
    float cos_table[360];

    void Init_lookup_tables();

    int get_table_index(float angle);

    float Sin_table(int index);
    float Cos_table(int index);

    
};

#endif // !MAP_H


