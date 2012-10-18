#pragma once
#ifndef idF3B3B5A5_BEF9_4137_A4460B6A94344DF6
#define idF3B3B5A5_BEF9_4137_A4460B6A94344DF6

#include <windows.h>

#define TRAY_ICON_ID 0xCAFEBABE

extern HINSTANCE g_hInstance;
extern NOTIFYICONDATA g_nidIcon;
void InitializeTray(void);
HICON GetApplicationIcon(void);

#endif
