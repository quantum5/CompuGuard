#include <windows.h>
#include <CompuGuard.h>

BOOL onSecureDesk;
HDESK hNewDesk;
HDESK hOldDesk;

void SetDesktop(HDESK hDesk) {
	SetThreadDesktop(hDesk);
	SwitchDesktop(hDesk);
}

void EnterSecureDesk() {
	TCHAR szDeskName[40];
	
	if (onSecureDesk)
		return;
	
	GenerateUUID(szDeskName);
	
	hOldDesk = GetThreadDesktop(GetCurrentThreadId());
	hNewDesk = CreateDesktop(szDeskName, NULL, NULL, 0, GENERIC_ALL, NULL);
	SetDesktop(hNewDesk);
}

void ExitSecureDesk() {
	SetDesktop(g_hOldDesk);
}
