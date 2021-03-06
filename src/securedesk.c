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
	
	GenerateUUID(szDeskName, 40);
	
	hOldDesk = GetThreadDesktop(GetCurrentThreadId());
	hNewDesk = CreateDesktop(szDeskName, NULL, NULL, 0, GENERIC_ALL, NULL);
	SetDesktop(hNewDesk);
	
	onSecureDesk = TRUE;
}

void ExitSecureDesk() {
	if (!onSecureDesk)
		return;
	SetDesktop(hOldDesk);
	onSecureDesk = FALSE;
}
