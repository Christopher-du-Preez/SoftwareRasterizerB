#include <stdio.h>
#include <render.h>

int main(void){

    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("Failed to Initialize");
        return 1;
    }
    
    SDL_Window *window = SDL_CreateWindow("SoftwareRasterizerB", 640, 480, SDL_WINDOW_RESIZABLE);
    if(!window){
        SDL_Log("Failed to create window");
        return 2;
    }

    bool isRunning = true;
    SDL_Event event;
    SDL_Surface *window_surface = SDL_GetWindowSurface(window);

    uint64_t frames = 0, lastTime = 0;
    while(isRunning){

        uint64_t currentTime = SDL_GetTicks();
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_EVENT_QUIT)
                isRunning = false;
        }

        Render(window_surface);
        SDL_UpdateWindowSurface(window);

        frames++;
        uint64_t deltaTime = SDL_GetTicks() - currentTime;
        if(currentTime > lastTime + 1000){
            lastTime = currentTime;
            char sframes[40];
            sprintf(sframes, "SoftwareRasterizerB FPS: %i", frames);
            SDL_SetWindowTitle(window, sframes);
            frames = 0;
        }
    }

    SDL_Quit();
    return 0;
}