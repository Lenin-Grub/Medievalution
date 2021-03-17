#include "stdafx.h"
#include "Game.h"

#define __CRTDBG_MAP_ALLOC
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW

int main()
{
	Game game;
	game.run();
	return 0;

	_CrtDumpMemoryLeaks();
}
