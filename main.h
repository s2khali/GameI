#pragma once

// GLOBAL VARIABLES
#define G__GAME_NAME "GameI"
#define DEBUG FALSE

#define G__DEFAULT_RESOLUTION_WIDTH     1280
#define G__DEFAULT_RESOLUTION_HEIGHT    720
#define G__DEFAULT_BITS_PER_PIXEL       32
#define G__DRAWING_AREA_MEMORY_SIZE     (G__DEFAULT_RESOLUTION_WIDTH * G__DEFAULT_RESOLUTION_HEIGHT * (G__DEFAULT_BITS_PER_PIXEL / 8))

typedef struct GAMEBITMAP {
    void* MemoryLocation;
    BITMAPINFO bitmapInfo;

} GAMEBITMAP;

LRESULT CALLBACK MainWindowProc(_In_ HWND windowHandle,_In_ UINT message,_In_ WPARAM wParam,_In_ LPARAM lParam);
DWORD CreateMainGameWindow(void);
BOOL GameIsAlreadyRunning(void);
void ProcessPlayerInput(void);
void RenderFrameGraphics(void);
