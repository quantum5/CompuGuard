#pragma once
#ifndef idF3B3B5A5_BEF9_4137_A4460B6A94344DF6
#define idF3B3B5A5_BEF9_4137_A4460B6A94344DF6

#include <windows.h>

#ifdef T
#undef T
#endif

#define T(s) TEXT(s)

#define APP_NAME T("CompuGuard")
#define OPTION_WIN_CLASS T("CompuGuardOptions")
#define TRAY_ICON_ID 0xCAFEBABE
#define WM_TRAY (WM_APP+1)
#define MessageErrorWnd(hwnd, e) MessageBox(hwnd, e, T("Error!"), MB_ICONERROR)
#define MessageError(e) MessageErrorWnd(NULL, e)
#define LockIfCreated(cs) do { if (cs) EnterCriticalSection(cs); } while (0)
#define UnlockIfCreated(cs) do { if (cs) LeaveCriticalSection(cs); } while (0)

extern HINSTANCE g_hInstance;
extern NOTIFYICONDATA g_nidIcon;
extern HWND g_hwOptions;
extern CRITICAL_SECTION *g_csTray;

void InitializeTray(void);
void InitializeOptions(void);
void UninitializeTray(void);
HICON GetApplicationIcon(void);

#endif
