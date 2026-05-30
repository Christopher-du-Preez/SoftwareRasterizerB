#include <app.h>

int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE p_instance, LPSTR lpCmdLine, int nCmdShow)
{
    if (!init())
    {
        return -1;
    }
    run();
    shut();
    return 0;
}