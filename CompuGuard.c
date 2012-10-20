#include <windows.h>
#include "CompuGuard.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MSG msg;

	g_hInstance = hInstance;
	if (!InitializeCriticalSectionAndSpinCount(g_csTray, 256))
		g_csTray = NULL;

	InitializeOptions();
	InitializeTray();
	ShowWindow(g_hwOptions, SW_SHOW);
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UninitializeTray();
	return msg.wParam;
}
