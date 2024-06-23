#ifndef APPLICATION_H
#define APPLICATION_H
#include "olcPixelGameEngine.h"
#include "map.h"
#include "player.h"
#include "ray.h"
#include "wall.h"
#include "sprite.h"




class Application
{
public:
	Application() = default;

public:
	Map map;
	Player player;
	Rays ray;
	Wall wall;
	Sprite sprite;

public:

	void Setup();

	void ProcessInput(olc::PixelGameEngine* pge);

	void Update(float Deltatime);

	void Render(olc::PixelGameEngine* pge);

};

#endif // !APPLICATION_H

