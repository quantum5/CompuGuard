#include <windows.h>
#include <strsafe.h>
#include <shellapi.h>
#include "CompuGuard.h"

void InitializeTray(void) {
	LockIfCreated(g_csTray);

	g_nidIcon.cbSize = sizeof(NOTIFYICONDATA);
	g_nidIcon.dwInfoFlags = NIIF_INFO;
	g_nidIcon.hIcon = GetApplicationIcon();
	g_nidIcon.hWnd = g_hwOptions;
	g_nidIcon.uCallbackMessage = WM_TRAY;
	g_nidIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	g_nidIcon.uID = TRAY_ICON_ID;
	g_nidIcon.uTimeout = 10000;
	StringCchCopy(g_nidIcon.szInfoTitle, 64, APP_NAME);
	Shell_NotifyIcon(NIM_ADD, &g_nidIcon);

	g_nidIcon.uVersion = NOTIFYICON_VERSION;
	Shell_NotifyIcon(NIM_SETVERSION, &g_nidIcon);

	UnlockIfCreated(g_csTray);
}

void UninitializeTray(void) {
	Shell_NotifyIcon(NIM_DELETE, &g_nidIcon);
}
