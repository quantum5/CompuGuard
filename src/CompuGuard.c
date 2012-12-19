#include <windows.h>
#include "CompuGuard.h"

// in instance.c
BOOL LockCreation();
void UnlockCreation();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MSG msg;
	HMODULE user32;

	if (!LockCreation()) {
		if (MessageBox(NULL, T("Another instance is already running\r\n")
				T("Do you still want to create a new instance?"),
				T("Another Instance Running"),
				MB_YESNO) == IDNO) {
			ExitProcess(0);
		}
	}
	g_hInstance = hInstance;
	g_hHeap = GetProcessHeap();
	g_hFont = CreateFont(18, 0, 0, 0, FW_REGULAR,
							FALSE, FALSE, FALSE, DEFAULT_CHARSET,
							OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, NULL);
	g_hBrush = CreateSolidBrush(RGB(0xF0, 0xF0, 0xF0));

	ProtectProcess();
	InitializeAccessibilityShortcutKeys();

	user32 = GetModuleHandle(T("user32"));
	fShutdownBlockReasonCreate = (LPFN_SHUTDOWNBLOCKREASONCREATE)GetProcAddress(user32, "ShutdownBlockReasonCreate");
	fShutdownBlockReasonDestroy = (LPFN_SHUTDOWNBLOCKREASONDESTROY)GetProcAddress(user32, "ShutdownBlockReasonDestroy");

	InitializeOptions();
	InitializeTray();
	InitializeBSOD();

	g_hBSODaccel = CreateAcceleratorTable((LPACCEL)&BSODaccel, BSOD_ACCEL_SIZE);

	ShowWindow(g_hwOptions, nCmdShow);
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		if (hBSODwnd && TranslateAccelerator(hBSODwnd, g_hBSODaccel, &msg))
			continue;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UninitializeTray();
	DeleteObject(g_hBrush);
	UnlockCreation();
	return msg.wParam;
}
