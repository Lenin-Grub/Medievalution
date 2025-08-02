#include "Application/Application.hpp"

#ifdef NDEBUG
    #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

int main()
{
    EditorApplication application;
    application.run();
    return 0;
}