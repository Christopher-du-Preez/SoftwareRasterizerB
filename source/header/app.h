#include <SDL3/SDL.h>

#define FALSE 0
#define TRUE 1

typedef struct handler
{
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_Event event;
} handler_t;

uint8_t init();
uint8_t poll_event(SDL_Event *event);
void run();
void shutdown();