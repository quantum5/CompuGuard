#include "CompuGuard.h"
#include <windows.h>
#include <aclapi.h>

/* http://stackoverflow.com/a/10575889/1090657 */
DWORD ProtectProcess(void) {
	HANDLE hProcess = GetCurrentProcess();
	PACL pEmptyDacl;
	DWORD dwErr;

	pEmptyDacl = (PACL) SelfAlloc(sizeof(ACL));
	if (!InitializeAcl(pEmptyDacl, sizeof(ACL), ACL_REVISION)) {
		dwErr = GetLastError();
		MessageLastErrorWndTitle(NULL, dwErr, "Failed to InitializeAcl()");
		return dwErr;
	}
	dwErr = SetSecurityInfo(hProcess, SE_KERNEL_OBJECT, 
							DACL_SECURITY_INFORMATION, NULL,
							NULL, pEmptyDacl, NULL);
	if (dwErr != ERROR_SUCCESS) {
		MessageLastErrorWndTitle(NULL, GetLastError(), "Failed to SetSecurityInfo()");
		return dwErr;
	}
	SelfFree(pEmptyDacl);
	return dwErr;
}

