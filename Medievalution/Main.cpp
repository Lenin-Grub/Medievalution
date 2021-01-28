#include "stdafx.h"
#include "Game.h"

//#define __CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new DEBUG_NEW

int main()
{
	Game game;
	game.run();
	//_CrtDumpMemoryLeaks();
	return 0;
}