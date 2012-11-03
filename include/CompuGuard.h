#pragma once
#ifndef idF3B3B5A5_BEF9_4137_A4460B6A94344DF6
#define idF3B3B5A5_BEF9_4137_A4460B6A94344DF6

#include <windows.h>
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

#define LockIfCreated(cs) do { if (cs) EnterCriticalSection(cs); } while (0)
#define UnlockIfCreated(cs) do { if (cs) LeaveCriticalSection(cs); } while (0)

#define MessageErrorWnd(hwnd, e) MessageBox(hwnd, e, T("Error!"), MB_ICONERROR)
#define MessageError(e) MessageErrorWnd(NULL, e)

#ifdef DEBUG
#   define MessageLastErrorWnd(hwnd, e) do { \
        LPTSTR s; \
        if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, e, 0, (LPTSTR)&s, 0, NULL) == 0) {\
            TCHAR str[25]; \
            StringCchPrintf(str, 25, T("%x"), str); \
            MessageErrorWnd(hwnd, str); \
        } else { \
            MessageErrorWnd(hwnd, s); \
            LocalFree(s); \
        } \
    } while (0)
#else
#   define MessageLastErrorWnd(hwnd, e)
#endif

#define MessageLastError(e) MessageLastErrorWnd(NULL, e)
#define SelfAlloc(size) HeapAlloc(g_hHeap, 0, size)
#define SelfAllocZero(size) HeapAlloc(g_hHeap, HEAP_ZERO_MEMORY, size)
#define SelfFree(ram) = HeapFree(g_hHeap, 0, ram)


#define TRAYBTN_SHOW 0xDE00
#define TRAYBTN_EXIT 0xDE01

#define OPTBTN_PREVENT_SHUTDOWN 0xAD01
#define OPTBTN_PREVENT_SLEEP 0xAD02

typedef BOOL (WINAPI *LPFN_SHUTDOWNBLOCKREASONCREATE) (HWND, LPCWSTR);
typedef BOOL (WINAPI *LPFN_SHUTDOWNBLOCKREASONDESTROY) (HWND);

extern HINSTANCE g_hInstance;
extern NOTIFYICONDATA g_nidIcon;
extern HWND g_hwOptions;
extern CRITICAL_SECTION *g_csTray;
extern HANDLE g_hHeap;
extern HFONT g_hFont;

extern LPFN_SHUTDOWNBLOCKREASONCREATE fShutdownBlockReasonCreate;
extern LPFN_SHUTDOWNBLOCKREASONDESTROY fShutdownBlockReasonDestroy;

void InitializeTray(void);
void InitializeOptions(void);
void UninitializeTray(void);
HICON GetApplicationIcon(void);
void ShowTrayMenu(HWND hwnd);
INT_PTR ShowPasswordDialog(HWND hwnd);

#endif
