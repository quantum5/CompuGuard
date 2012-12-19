#include <windows.h>
#include <shellapi.h>
#include "CompuGuard.h"

void InitializeTray(void) {
	g_nidIcon.cbSize = sizeof(NOTIFYICONDATA);
	/*g_nidIcon.dwInfoFlags = NIIF_INFO;*/
	g_nidIcon.hIcon = GetApplicationIcon();
	g_nidIcon.hWnd = g_hwOptions;
	g_nidIcon.uCallbackMessage = WM_TRAY;
	g_nidIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	g_nidIcon.uID = TRAY_ICON_ID;
	lstrcpy(g_nidIcon.szInfoTitle, APP_NAME);
	Shell_NotifyIcon(NIM_ADD, &g_nidIcon);
}

void UninitializeTray(void) {
	Shell_NotifyIcon(NIM_DELETE, &g_nidIcon);
}

void ShowTrayMenu(HWND hwnd) {
	POINT loc;
	HMENU menu = CreatePopupMenu();
	
	GetCursorPos(&loc);
	AppendMenu(menu, MF_STRING, TRAYBTN_SHOW, IsWindowVisible(hwnd) ? T("&Hide") : T("&Show"));
	AppendMenu(menu, MF_STRING, TRAYBTN_EXIT, T("&Exit"));
	
	SetMenuDefaultItem(menu, TRAYBTN_SHOW, FALSE);
	SetForegroundWindow(hwnd);
	TrackPopupMenu(menu, TPM_BOTTOMALIGN, loc.x, loc.y, 0, hwnd, NULL);
	DestroyMenu(menu);
}
