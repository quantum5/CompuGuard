#include "CompuGuard.h"
#include <windows.h>

LRESULT CALLBACK OptionsWndProc(HWND, UINT, WPARAM, LPARAM);

void InitializeOptions(void) {
	WNDCLASSEX wincl;
	HWND hwnd;

	wincl.hInstance = g_hInstance;
	wincl.lpszClassName = OPTION_WIN_CLASS;
	wincl.lpfnWndProc = OptionsWndProc;
	wincl.style = CS_DBLCLKS;
	wincl.cbSize = sizeof(WNDCLASSEX);

	wincl.hIcon = GetApplicationIcon();
	wincl.hIconSm = GetApplicationIcon();
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;
	wincl.cbClsExtra = 0;
	wincl.cbWndExtra = 0;
	wincl.hbrBackground = (HBRUSH) GetStockObject(LTGRAY_BRUSH);

	if(!RegisterClassEx(&wincl)) {
		MessageError(T("Can't register windows class '") OPTION_WIN_CLASS T("'!"));
		ExitProcess(0);
	}

	hwnd = CreateWindowEx(0, OPTION_WIN_CLASS, T("Computer Guard Options"),
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
			CW_USEDEFAULT, 640, 480,
			NULL, NULL, g_hInstance, NULL);

	UpdateWindow(hwnd);
	g_hwOptions = hwnd;
}

LRESULT CALLBACK OptionsWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_CREATE:
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_TRAY:
			switch (lParam) {
				case WM_LBUTTONDBLCLK:
					ShowWindow(hwnd, IsWindowVisible(hwnd) ? SW_HIDE : SW_SHOW);
					break;
			}
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}