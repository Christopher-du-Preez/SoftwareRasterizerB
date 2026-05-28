#include <app.h>
#include <render.h>
#include <stdio.h>

handler_t handler = { 0 };

uint8_t init()
{
     if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("Failed to Initialize");
        return FALSE;
    }
    
    handler.window = SDL_CreateWindow("SoftwareRasterizerB", 640, 480, SDL_WINDOW_RESIZABLE);
    if(!handler.window){
        SDL_Log("Failed to create window");
        return FALSE;
    }
    handler.surface = SDL_GetWindowSurface(handler.window);   
    
    return TRUE;
}

uint8_t poll_event(SDL_Event *event)
{
    while(SDL_PollEvent(event)){
            if(event->type == SDL_EVENT_QUIT)
                return FALSE;
        }
    return TRUE;
}

void run()
{
    uint64_t frames = 0, lastTime = 0;
    while(TRUE){

        uint64_t currentTime = SDL_GetTicks();

        if(!poll_event(&handler.event))
            break;
        render();

        frames++;
        uint64_t deltaTime = SDL_GetTicks() - currentTime;
        if(currentTime > lastTime + 1000){
            lastTime = currentTime;
            char sframes[40];
            sprintf(sframes, "SoftwareRasterizerB FPS: %li", frames);
            SDL_SetWindowTitle(handler.window, sframes);
            frames = 0;
        }
    }
}

void shutdown()
{
    SDL_DestroySurface(handler.surface);
    SDL_DestroyWindow(handler.window);
    SDL_Quit();
}