#include <app.h>
#include <render.h>

handler_t handler;
extern uint32_t* frame_buffer;

uint8_t init()
{
    memset(&handler, 0, sizeof(handler));
    
    handler.h_instance = GetModuleHandle(0);
    handler.state = TRUE; 

    AllocConsole();

    handler.console = GetStdHandle(STD_OUTPUT_HANDLE);

    WNDCLASS wc;
    memset(&wc, 0, sizeof(wc));
    wc.lpfnWndProc = win_proc;
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

    frame_buffer = calloc(sizeof(uint32_t), WIDTH * HEIGHT);
    if (!frame_buffer)
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
    handler.bitmap = CreateDIBSection(NULL, &bi, DIB_RGB_COLORS, &frame_buffer, 0, 0);
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

LRESULT CALLBACK win_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
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
    render_init();
    while(handler.state){

        poll_event();
        render();
        InvalidateRect(handler.window, NULL, FALSE);
        UpdateWindow(handler.window);
    }
}

void shut()
{
    FreeConsole();
    if (handler.window)
    {
        DestroyWindow(handler.window);
        handler.window = 0;
    }
}