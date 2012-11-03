#include "CompuGuard.h"
#include <windows.h>
#include <aclapi.h>

DWORD ProtectProcess() {
	HANDLE hProcess = GetCurrentProcess();
	EXPLICIT_ACCESS denyAccess = {0};
	DWORD dwAccessPermissions = GENERIC_WRITE | PROCESS_ALL_ACCESS |
								WRITE_DAC | DELETE | WRITE_OWNER |
								READ_CONTROL;
	PACL pTempDacl = NULL;
	DWORD dwErr = 0;
	
	BuildExplicitAccessWithName(&denyAccess, T("CURRENT_USER"), dwAccessPermissions, DENY_ACCESS, NO_INHERITANCE);
	dwErr = SetEntriesInAcl(1, &denyAccess, NULL, &pTempDacl);
	if (dwErr != ERROR_SUCCESS) {
		MessageLastErrorWndTitle(NULL, GetLastError(), "Failed to SetEntriesInAcl()");
		return dwErr;
	}
	dwErr = SetSecurityInfo(hProcess, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, pTempDacl, NULL);
	if (dwErr != ERROR_SUCCESS) {
		MessageLastErrorWndTitle(NULL, GetLastError(), "Failed to SetSecurityInfo()");
		return dwErr;
	}
	LocalFree(pTempDacl);
	CloseHandle(hProcess);
	return ERROR_SUCCESS;
}
