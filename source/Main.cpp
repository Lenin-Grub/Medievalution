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


//
//#include <SFML/Window.hpp>
//#include <SFML/OpenGL.hpp>
//
//int main()
//{
//    sf::ContextSettings settings;
//settings.depthBits = 24;
//settings.stencilBits = 8;
//settings.antialiasingLevel = 4;
//settings.majorVersion = 3;
//settings.minorVersion = 0;
//    // create the window
//    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, settings);
//    window.setVerticalSyncEnabled(true);
//
//    // activate the window
//    window.setActive(true);
//
//    // load resources, initialize the OpenGL states, ...
//
//    // run the main loop
//    bool running = true;
//    while (running)
//    {
//        // handle events
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//            {
//                // end the program
//                running = false;
//            }
//            else if (event.type == sf::Event::Resized)
//            {
//                // adjust the viewport when the window is resized
//                glViewport(0, 0, event.size.width, event.size.height);
//            }
//        }
//
//        // clear the buffers
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//
//        float size = 0.25;
//
//        glBegin(GL_QUADS);
//        glVertex3f(-size, -size, -size); glTexCoord2f(0, 0);
//        glVertex3f(size, -size, -size); glTexCoord2f(0, 1);
//        glVertex3f(size, size, -size); glTexCoord2f(1, 1);
//        glVertex3f(-size, size, -size); glTexCoord2f(1, 0);
//
//        glVertex3f(size, -size, size); glTexCoord2f(0, 0);
//        glVertex3f(-size, -size, size); glTexCoord2f(0, 1);
//        glVertex3f(-size, size, size);  glTexCoord2f(1, 1);
//        glVertex3f(size, size, size);  glTexCoord2f(1, 0);
//
//        glVertex3f(-size, -size, size); glTexCoord2f(0, 0);
//        glVertex3f(-size, -size, -size); glTexCoord2f(0, 1);
//        glVertex3f(-size, size, -size); glTexCoord2f(1, 1);
//        glVertex3f(-size, size, size); glTexCoord2f(1, 0);
//
//        glVertex3f(size, -size, -size); glTexCoord2f(0, 0);
//        glVertex3f(size, -size, size); glTexCoord2f(0, 1);
//        glVertex3f(size, size, size); glTexCoord2f(1, 1);
//        glVertex3f(size, size, -size); glTexCoord2f(1, 0);
//        glEnd();
//
//        window.display();
//    }
//
//    // release resources...
//
//    return 0;
//}
