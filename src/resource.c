#include <windows.h>
#include "CompuGuard.h"
#include "resource.h"

HICON GetApplicationIcon(void) {
#ifdef NORES
	return (HICON)LoadImage(g_hInstance,
							MAKEINTRESOURCE(IDI_MY_ICON),
							IMAGE_ICON,
							GetSystemMetrics(SM_CXSMICON),
							GetSystemMetrics(SM_CYSMICON),
							LR_DEFAULTCOLOR);
#else
	return (HICON)LoadImage(g_hInstance, T("guard.ico"),
							IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
#endif
}
