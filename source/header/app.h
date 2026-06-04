#include <windows.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define HEIGHT 480
#define WIDTH 640
#define BORDER_OFF_Y 39
#define BORDER_OFF_X 16

typedef struct handler
{
    HWND window;
    HINSTANCE h_instance;
    HDC mem_dc;
    HBITMAP bitmap;
    uint32_t *frame_buffer;
    uint8_t state;
} handler_t;

uint8_t init();
void poll_event();
LRESULT CALLBACK win_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
void run();
void shut();