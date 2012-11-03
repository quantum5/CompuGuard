/* This file initialized global variables for this project */
#include "CompuGuard.h"
#include <windows.h>

HINSTANCE g_hInstance;
NOTIFYICONDATA g_nidIcon;
HWND g_hwOptions;
CRITICAL_SECTION g_csTray_;
CRITICAL_SECTION* g_csTray = &g_csTray_;
HANDLE hHeap;
