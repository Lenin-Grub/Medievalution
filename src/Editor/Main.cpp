#include "Application/Application.hpp"

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main()
{
    EditorApplication application;
    application.run();
    return 0;
}