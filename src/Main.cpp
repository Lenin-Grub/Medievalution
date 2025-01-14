#include "stdafx.h"
#include "Game/Game.h"

// Off console in release version.
#ifdef NDEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

int main()
{
    Game game;
    game.run();
    return 0;
}