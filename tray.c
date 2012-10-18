#include <windows.h>
#include "CompuGuard.h"

HICON GetApplicationIcon(void) {
#ifdef NORES
	return (HICON)LoadImage(g_hInstance,
							MAKEINTRESOURCE(IDI_MY_ICON),
							IMAGE_ICON,
							GetSystemMetrics(SM_CXSMICON),
							GetSystemMetrics(SM_CYSMICON),
							LR_DEFAULTCOLOR);
#else
	return (HICON)LoadImage(hInstance, _T(""), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);;
#endif
}

void InitializeTray(void) {
	g_nidIcon

	ULONGLONG ullVersion = GetDllVersion(_T("Shell32.dll"));
	if (ullVersion >= MAKEDLLVERULL(6, 0, 0, 0))
		g_nidIcon.cbSize = sizeof(NOTIFYICONDATA);
	else if (ullVersion >= MAKEDLLVERULL(5, 0, 0, 0))
		g_nidIcon.cbSize = NOTIFYICONDATA_V2_SIZE;
	else
		g_nidIcon.cbSize = NOTIFYICONDATA_V1_SIZE;
	g_nidIcon.uID = TRAY_ICON_ID;
	g_nidIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	niData.hIcon =
        

}
