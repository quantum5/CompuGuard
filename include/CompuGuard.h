#pragma once
#ifndef idF3B3B5A5_BEF9_4137_A4460B6A94344DF6
#define idF3B3B5A5_BEF9_4137_A4460B6A94344DF6

#include <windows.h>
#include <shellapi.h>
#include <strsafe.h>

#ifdef T
#undef T
#endif

#define T(s) TEXT(s)

#define APP_NAME T("CompuGuard")
#define OPTION_WIN_CLASS T("CompuGuardOptions")

#define TRAY_ICON_ID 0xCAFEBABE

#define WM_TRAY (WM_APP+1)
#define WM_EXIT (WM_APP+2)

#include "selfmemory.h"
#include "qdebug.h"

#define LockIfCreated(cs) do { if (cs) EnterCriticalSection(cs); } while (0)
#define UnlockIfCreated(cs) do { if (cs) LeaveCriticalSection(cs); } while (0)


#define TRAYBTN_SHOW 0xDE00
#define TRAYBTN_EXIT 0xDE01

#define OPTBTN_PREVENT_SHUTDOWN 0xAD01
#define OPTBTN_PREVENT_SLEEP 0xAD02

typedef BOOL (WINAPI *LPFN_SHUTDOWNBLOCKREASONCREATE) (HWND, LPCTSTR);
typedef BOOL (WINAPI *LPFN_SHUTDOWNBLOCKREASONDESTROY) (HWND);

extern HINSTANCE g_hInstance;
extern NOTIFYICONDATA g_nidIcon;
extern HWND g_hwOptions;
extern CRITICAL_SECTION *g_csTray;
extern HFONT g_hFont;

extern LPFN_SHUTDOWNBLOCKREASONCREATE fShutdownBlockReasonCreate;
extern LPFN_SHUTDOWNBLOCKREASONDESTROY fShutdownBlockReasonDestroy;

void InitializeTray(void);
void InitializeOptions(void);
void UninitializeTray(void);
HICON GetApplicationIcon(void);
void ShowTrayMenu(HWND hwnd);
INT_PTR ShowPasswordDialog(HWND hwnd);

void DisableShutdown(HWND hwnd);
void EnableShutdown(HWND hwnd);
void PreventShutdown(HWND hwnd, BOOL enabled);
void DisableSleep();
void EnableSleep();
void PreventSleep(BOOL enabled);

BOOL DisableMouse(void);
BOOL DisableKeyboard(void);
BOOL EnableMouse(void);
BOOL EnableKeyboard(void);

DWORD ProtectProcess();

#endif
