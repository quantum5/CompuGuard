#include <windows.h>
#include "CompuGuard.h"
#include "resource.h"

HICON GetApplicationIcon(void) {
	static HICON icon = INVALID_HANDLE_VALUE;
	if (icon == INVALID_HANDLE_VALUE) {
#ifndef NORES
		icon = (HICON)LoadImage(g_hInstance,
								MAKEINTRESOURCE(RID_LOCK),
								IMAGE_ICON,
								GetSystemMetrics(SM_CXSMICON),
								GetSystemMetrics(SM_CYSMICON),
								0);
#else
		icon = (HICON)LoadImage(g_hInstance, T("guard.ico"),
								IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
#endif
	}
	return icon;
}
