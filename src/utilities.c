#include <CompuGuard.h>
#include <rpc.h>

void GenerateUUID(LPTSTR szUUID, int size) {
	UUID bUuid;
#ifdef UNICODE
	RPC_WSTR rstrUUID;
#else
	RPC_CSTR rstrUUID;
#endif

	UuidCreate(&bUuid);
	UuidToString(&bUuid, &rstrUUID);
	lstrcpyn(szUUID, (LPCTSTR) rstrUUID, size);
	RpcStringFree(&rstrUUID);
}

void DisableTaskManager(void) {
	DWORD dwOne = 1;
	HKEY hSystemPolicy;
	if (!RegCreateKeyEx(HKEY_CURRENT_USER, T("Software\\Microsoft\\Windows\\")
						T("CurrentVersion\\Policies\\System"), 0, NULL,
						0, KEY_SET_VALUE, NULL, &hSystemPolicy, NULL))
		RegSetValueEx(hSystemPolicy, T("DisableTaskMgr"), 0, REG_DWORD, (LPBYTE)&dwOne, sizeof(DWORD));
}

void EnableTaskManager(void) {
	DWORD dwZero = 0;
	HKEY hSystemPolicy;
	if (!RegCreateKeyEx(HKEY_CURRENT_USER, T("Software\\Microsoft\\Windows\\")
						T("CurrentVersion\\Policies\\System"), 0, NULL,
						0, KEY_SET_VALUE, NULL, &hSystemPolicy, NULL))
		RegSetValueEx(hSystemPolicy, T("DisableTaskMgr"), 0, REG_DWORD, (LPBYTE)&dwZero, sizeof(DWORD));
}

STICKYKEYS StartupStickyKeys = {sizeof(STICKYKEYS), 0};
TOGGLEKEYS StartupToggleKeys = {sizeof(TOGGLEKEYS), 0};
FILTERKEYS StartupFilterKeys = {sizeof(FILTERKEYS), 0};

void InitializeAccessibilityShortcutKeys() {
	// Save the current sticky/toggle/filter key settings so they can be restored them later
	SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &StartupStickyKeys, 0);
	SystemParametersInfo(SPI_GETTOGGLEKEYS, sizeof(TOGGLEKEYS), &StartupToggleKeys, 0);
	SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS), &StartupFilterKeys, 0);
}

void AllowAccessibilityShortcutKeys(BOOL bAllowKeys) {
	if (bAllowKeys) {
		SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &StartupStickyKeys, 0);
		SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &StartupToggleKeys, 0);
		SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &StartupFilterKeys, 0);
	} else {
		// Disable StickyKeys/etc shortcuts but if the accessibility feature is on,
		// then leave the settings alone as its probably being usefully used
		STICKYKEYS skOff = StartupStickyKeys;
		TOGGLEKEYS tkOff = StartupToggleKeys;
		FILTERKEYS fkOff = StartupFilterKeys;
		if ((skOff.dwFlags & SKF_STICKYKEYSON) == 0) {
			// Disable the hotkey and the confirmation
			skOff.dwFlags &= ~SKF_HOTKEYACTIVE;
			skOff.dwFlags &= ~SKF_CONFIRMHOTKEY;
			SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &skOff, 0);
		}
		if ((tkOff.dwFlags & TKF_TOGGLEKEYSON) == 0) {
			// Disable the hotkey and the confirmation
			tkOff.dwFlags &= ~TKF_HOTKEYACTIVE;
			tkOff.dwFlags &= ~TKF_CONFIRMHOTKEY;
			SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &tkOff, 0);
		}
		if ((fkOff.dwFlags & FKF_FILTERKEYSON) == 0) {
			// Disable the hotkey and the confirmation
			fkOff.dwFlags &= ~FKF_HOTKEYACTIVE;
			fkOff.dwFlags &= ~FKF_CONFIRMHOTKEY;
			SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &fkOff, 0);
		}
	}
}
