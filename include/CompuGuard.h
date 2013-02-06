#pragma once
#ifndef idF3B3B5A5_BEF9_4137_A4460B6A94344DF6
#define idF3B3B5A5_BEF9_4137_A4460B6A94344DF6

#include <windows.h>
#include <shellapi.h>

#ifdef T
#undef T
#endif

#define T(s) TEXT(s)

#ifdef _MSC_VER
#define snprintf sprintf_s
#endif

#define APP_NAME T("CompuGuard")
#define OPTION_WIN_CLASS T("CompuGuardOptions")
#define BSOD_WIN_CLASS T("CompuGuardBSOD")

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
#define OPTBTN_BSOD_DISABLE_MOUSE 0xAD11
#define OPTBTN_BSOD_SECURE_DESK 0xAD12
#define OPTBTN_BSOD_NO_TASKMGR 0xAD13
#define OPTBTN_BSOD_RUN 0xAD1F

#define BSOD_ACCEL_SIZE 8
#define WM_ENFORCE_FOCUS (WM_APP+1)

typedef BOOL (WINAPI *LPFN_SHUTDOWNBLOCKREASONCREATE) (HWND, LPCTSTR);
typedef BOOL (WINAPI *LPFN_SHUTDOWNBLOCKREASONDESTROY) (HWND);

extern HINSTANCE g_hInstance;
extern NOTIFYICONDATA g_nidIcon;
extern HWND g_hwOptions;
extern CRITICAL_SECTION *g_csTray;
extern HFONT g_hFont;
extern HBRUSH g_hBrush;
extern HWND g_hPassDlg;

extern BOOL g_BSODnoMouse;
extern BOOL g_BSODsecureDesk;
extern BOOL g_BSODnotaskmgr;
extern BOOL g_BSODrunning;

extern HACCEL g_hBSODaccel;
extern HWND hBSODwnd;

extern LPFN_SHUTDOWNBLOCKREASONCREATE fShutdownBlockReasonCreate;
extern LPFN_SHUTDOWNBLOCKREASONDESTROY fShutdownBlockReasonDestroy;

extern ACCEL BSODaccel[BSOD_ACCEL_SIZE];
extern BOOL BSODbAccel[BSOD_ACCEL_SIZE];

void InitializeTray(void);
void InitializeOptions(void);
void UninitializeTray(void);
HICON GetApplicationIcon(void);
void ShowTrayMenu(HWND hwnd);
INT_PTR ShowPasswordDialog(HWND hwnd);
BOOL CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

void ShowBSOD(void);
void InitializeBSOD(void);

void SetDesktop(HDESK hDesk);
void EnterSecureDesk();
void ExitSecureDesk();

DWORD ProtectProcess();
void GenerateUUID(LPTSTR szUUID);
void DisableTaskManager(void);
void EnableTaskManager(void);
void InitializeAccessibilityShortcutKeys();
void AllowAccessibilityShortcutKeys(BOOL bAllowKeys);

#endif
