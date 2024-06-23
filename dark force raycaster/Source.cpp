#define OLC_PGE_APPLICATION

#include "application.h"

class Darkforce : public olc::PixelGameEngine
{
public:
	Darkforce()
	{
		sAppName = "dark force game";
	}
public:
	Application app;

public:

	bool OnUserCreate() override
	{
		app.Setup();


		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLACK);


		app.ProcessInput(this);
		app.Update(fElapsedTime);

		app.Render(this);


		return true;
	}
};

int main()
{
	Darkforce game;

	if (game.Construct(WINDOW_WIDTH, WINDOW_HEIGHT, PIXEL_SIZE, PIXEL_SIZE))
	{
		game.Start();
	}


	return 0;
}