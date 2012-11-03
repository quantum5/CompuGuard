#include <windows.h>
#include "CompuGuard.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MSG msg;
	HMODULE user32;

	g_hInstance = hInstance;
	if (!InitializeCriticalSectionAndSpinCount(g_csTray, 256))
		g_csTray = NULL;
	
	g_hHeap = GetProcessHeap();

	g_hFont = CreateFont(18, 0, 0, 0, FW_REGULAR,
							FALSE, FALSE, FALSE, DEFAULT_CHARSET,
							OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							CLEARTYPE_QUALITY, DEFAULT_PITCH|FF_SWISS, T("Segoe UI"));

	user32 = GetModuleHandle(T("user32"));
	fShutdownBlockReasonCreate = (LPFN_SHUTDOWNBLOCKREASONCREATE)GetProcAddress(user32, "ShutdownBlockReasonCreate");
	fShutdownBlockReasonDestroy = (LPFN_SHUTDOWNBLOCKREASONDESTROY)GetProcAddress(user32, "ShutdownBlockReasonDestroy");

	InitializeOptions();
	InitializeTray();
	ShowWindow(g_hwOptions, nCmdShow);
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UninitializeTray();
	return msg.wParam;
}