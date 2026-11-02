#include "window.h"



static HANDLE g_hInstance = NULL;
static HWND g_hWnd = NULL;
static windowinfo_t* g_winInfo = NULL;

LRESULT WINAPI window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch(uMsg){
        case WM_PAINT:{
            if(g_winInfo->onLoad != NULL){
                g_winInfo->onDraw();
            }
            break;
        }
        case WM_CLOSE:{
            DestroyWindow(hWnd);
            break;
        }
        case WM_DESTROY:{
            PostQuitMessage(0);
            break;
        }
        case WM_SIZE:{
            size_t width = LOWORD(lParam);
            size_t height = HIWORD(lParam);
            if(g_winInfo->onResize != NULL){
                g_winInfo->onResize(width, height);
            }
            break;
        }
        case WM_MOUSEMOVE:{
            break;
        }
        case WM_LBUTTONDOWN:{
            break;
        }
        case WM_RBUTTONDOWN:{
            break;
        }
        case WM_LBUTTONUP:{
            break;
        }
        case WM_RBUTTONUP:{
            break;
        }
        case WM_KEYDOWN:{
            break;
        }
        case WM_KEYUP:{
            break;
        }
        case WM_CHAR:{
            break;
        }
        default:{
            break;
        }
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool win_create_window(windowinfo_t* winInfo){
    if(winInfo == NULL) return FALSE;
    g_winInfo = winInfo;

    const char* className = "OpenIndevWinClass";
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = window_proc;
    /* is this reliable? not sure but oh well ig we shall see! */
    g_hInstance = GetModuleHandle(NULL);
    wc.hInstance = g_hInstance;
    wc.lpszClassName = className;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassA(&wc);
    g_hWnd = CreateWindowExA(
        0,
        className,
        g_winInfo->title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, g_winInfo->width, g_winInfo->height,
        NULL,
        NULL,
        g_hInstance,
        NULL
    );
    if(g_hWnd == NULL){
        return FALSE;
    }

    if(g_winInfo->onLoad != NULL){
        g_winInfo->onLoad();
    }

    ShowWindow(g_hWnd, SW_SHOW);
    UpdateWindow(g_hWnd);
    MSG msg = {0};

    while(GetMessage(&msg, NULL, 0, 0) != 0){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        InvalidateRect(g_hWnd, NULL, TRUE);
    }
    if(g_winInfo->onLoad != NULL){
        g_winInfo->onClose();
    }

    return TRUE;
}


const windowinfo_t* win_return_window_info(){
    return g_winInfo;
}

HWND win_return_hwnd(){
    return g_hWnd;
}
