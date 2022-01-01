#include <stdio.h>
#include <windows.h>

#include "main.h"

HWND g__GameWindow;
BOOL g__GameIsRunning;
GAMEBITMAP  g__DrawingSurface;


int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, PSTR cmdLine, INT cmdShow) {

    UNREFERENCED_PARAMETER(instance);
    UNREFERENCED_PARAMETER(previousInstance);
    UNREFERENCED_PARAMETER(cmdLine);
    UNREFERENCED_PARAMETER(cmdShow);

    if(GameIsAlreadyRunning() == TRUE) {
//        MessageBoxA(NULL, G__GAME_NAME " already running", "Error!", MB_ICONEXCLAMATION | MB_OK);

        goto Exit;
    }

    if(CreateMainGameWindow() != ERROR_SUCCESS) {
        goto Exit;
    }

    g__DrawingSurface.bitmapInfo.bmiHeader.biSize           = sizeof(g__DrawingSurface.bitmapInfo.bmiHeader);
    g__DrawingSurface.bitmapInfo.bmiHeader.biWidth          = G__DEFAULT_RESOLUTION_WIDTH;
    g__DrawingSurface.bitmapInfo.bmiHeader.biHeight         = G__DEFAULT_RESOLUTION_HEIGHT;
    g__DrawingSurface.bitmapInfo.bmiHeader.biBitCount       = G__DEFAULT_BITS_PER_PIXEL;
    g__DrawingSurface.bitmapInfo.bmiHeader.biCompression    = BI_RGB;
    g__DrawingSurface.bitmapInfo.bmiHeader.biPlanes         = 1;

    if((g__DrawingSurface.MemoryLocation = VirtualAlloc(NULL, G__DRAWING_AREA_MEMORY_SIZE,
                                                        MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)) == NULL) {
        MessageBoxA(NULL, "Failed to allocate memory!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    MSG message =  { 0 };

    g__GameIsRunning =  TRUE;

    // MAIN GAME LOOP
    while (g__GameIsRunning) {
        while(PeekMessageA(&message, g__GameWindow, 0, 0, PM_REMOVE)) {
            DispatchMessageA(&message);
        }

        ProcessPlayerInput();

        RenderFrameGraphics();

        Sleep(1); // TODO: Revisit
    }

Exit:
    return 0;
}

LRESULT CALLBACK MainWindowProc(_In_ HWND windowHandle,_In_ UINT message,_In_ WPARAM wParam,_In_ LPARAM lParam) {

    LRESULT result = 0;

    switch (message) {

        case WM_CLOSE: {
            g__GameIsRunning = FALSE;
            PostQuitMessage(0);

            break;
        }

        default: {
            result = DefWindowProcA(windowHandle, message, wParam, lParam);
            break;
        }
    }

    return result;
}

DWORD CreateMainGameWindow(void) {

    DWORD result = ERROR_SUCCESS;

    WNDCLASSEXA windowClass = { 0 };

    windowClass.cbSize = sizeof (WNDCLASSEXA);
    windowClass.style = 0;
    windowClass.lpfnWndProc = MainWindowProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = GetModuleHandleA(NULL);
    windowClass.hIcon = LoadIconA(NULL, IDI_APPLICATION);
    windowClass.hIconSm = LoadIconA(NULL, IDI_APPLICATION);
    windowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = G__GAME_NAME "_WINDOWCLASS";

    if(RegisterClassExA(&windowClass) == 0) {
        result = GetLastError();

        MessageBoxA(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

        goto Exit;
    }

    g__GameWindow = CreateWindowExA(WS_EX_CLIENTEDGE, windowClass.lpszClassName,
                                   G__GAME_NAME, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
                                   G__DEFAULT_RESOLUTION_WIDTH, G__DEFAULT_RESOLUTION_HEIGHT, NULL,
                                   NULL, GetModuleHandleA(NULL), NULL);

    if(g__GameWindow == NULL) {
        result = GetLastError();

        MessageBoxA(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

        goto Exit;
    }

Exit:
    return result;
}

BOOL GameIsAlreadyRunning(void) {
    HANDLE Mutex = NULL;

    Mutex = CreateMutexA(NULL, FALSE, G__GAME_NAME "_Mutex");

    if(GetLastError() == ERROR_ALREADY_EXISTS) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void ProcessPlayerInput(void) {
    short escapeKeyIsDown = GetAsyncKeyState(VK_ESCAPE);

    if(escapeKeyIsDown) {
        SendMessageA(g__GameWindow, WM_CLOSE, 0, 0);
    }
}

void RenderFrameGraphics(void) {

}