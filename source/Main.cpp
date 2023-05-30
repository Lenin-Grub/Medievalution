#include "stdafx.h"
#include "Game/Game.h"

//отключает консоль на релизной версии
#ifdef NDEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

int main()
{
	Game game;
	game.run();
	return 0;
}
