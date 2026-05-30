#include <app.h>
#include <render.h>

handler_t handler;

uint8_t init()
{
    memset(&handler, 0, sizeof(handler));
    
    handler.h_instance = GetModuleHandle(0);
    handler.state = TRUE; 

    WNDCLASS wc;
    memset(&wc, 0, sizeof(wc));
    wc.lpfnWndProc = update;
    wc.hInstance = handler.h_instance;
    wc.lpszClassName = "main";
    if (!RegisterClass(&wc))
    {
        return FALSE;
    }
    
    handler.window = CreateWindowEx(
        WS_EX_APPWINDOW,
        wc.lpszClassName,
        "SoftwareRasterizerB",
        WS_OVERLAPPED | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, WIDTH + BORDER_OFF_X, HEIGHT + BORDER_OFF_Y,
        NULL,
        NULL,
        handler.h_instance,
        NULL
    );
    if (!handler.window)
        return FALSE;

    handler.frame_buffer = calloc(sizeof(uint32_t), WIDTH * HEIGHT);
    if (!handler.frame_buffer)
    {
        shut();
        return FALSE;
    }
    handler.mem_dc = CreateCompatibleDC(NULL);
    if (!handler.mem_dc)
    {
        shut();
        return FALSE;
    }

    BITMAPINFO bi;
    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth = WIDTH;
    bi.bmiHeader.biHeight = -HEIGHT;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    handler.bitmap = CreateDIBSection(NULL, &bi, DIB_RGB_COLORS, &handler.frame_buffer, NULL, NULL);
    if (!handler.bitmap)
    {
        shut();
        return FALSE;
    }
    SelectObject(handler.mem_dc, handler.bitmap);
    
    ShowWindow(handler.window, SW_SHOW);
    
    return TRUE;
}

void poll_event()
{
    MSG msg;
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK update(HWND hwnd, UINT32 msg, WPARAM w_param, LPARAM l_param)
{
    switch (msg)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        handler.state = FALSE;
        return 0;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BeginPaint(handler.window, &ps);
        BitBlt(ps.hdc, 0, 0, WIDTH, HEIGHT, handler.mem_dc, 0, 0, SRCCOPY);
        EndPaint(handler.window, &ps);
        return 0;
    }
    }
    return DefWindowProc(hwnd, msg, w_param, l_param);
}

void run()
{
    uint32_t frames = 0, last_time = 0;
    while(handler.state){

        uint32_t current_time = SDL_GetTicks();

        poll_event();
        render();
        InvalidateRect(handler.window, NULL, FALSE);
        UpdateWindow(handler.window);

        frames++;
        uint32_t deltaTime = SDL_GetTicks() - current_time;
        if(current_time > last_time + 1000){
            //PRINT("");
            last_time = current_time;
            frames = 0;
        }
    }
}

void shut()
{
    if (handler.window)
    {
        DestroyWindow(handler.window);
        handler.window = 0;
    }
}