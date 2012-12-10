#include "CompuGuard.h"
#include <windows.h>

#ifndef ES_AWAYMODE_REQUIRED
#	define ES_AWAYMODE_REQUIRED 0x00000040
#endif

#ifdef __MINGW32__
DWORD SetThreadExecutionState(DWORD esFlags) {
	HMODULE dll = GetModuleHandle("kernel32");
	DWORD (__stdcall *func)(DWORD) = GetProcAddress(dll, "SetThreadExecutionState");
	return func(esFlags);
}
#endif

void DisableShutdown(HWND hwnd) {
	if (fShutdownBlockReasonCreate != NULL)
		fShutdownBlockReasonCreate(hwnd, T("Jon Skeet said you can't shutdown!"));
}

void EnableShutdown(HWND hwnd) {
	if (fShutdownBlockReasonDestroy != NULL)
		fShutdownBlockReasonDestroy(hwnd);
}

void PreventShutdown(HWND hwnd, BOOL enabled) {
	if (enabled)
		DisableShutdown(hwnd);
	else
		EnableShutdown(hwnd);
}

void DisableSleep() {
	SetThreadExecutionState(ES_CONTINUOUS |
							ES_SYSTEM_REQUIRED |
							ES_AWAYMODE_REQUIRED |
							ES_USER_PRESENT);
}

void EnableSleep() {
	SetThreadExecutionState(ES_CONTINUOUS);
}

void PreventSleep(BOOL enabled) {
	if (enabled)
		DisableSleep();
	else
		EnableSleep();
}
