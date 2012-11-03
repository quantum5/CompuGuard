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
	wincl.hbrBackground = CreateSolidBrush(RGB(0xF0, 0xF0, 0xF0));

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
	static HWND hwAppDesc, hwcPreventShutdown, hwcPreventSleep;
	switch (message) {
		case WM_CREATE:
			hwAppDesc = CreateWindow(T("STATIC"),
									 T("Welcome to CompuGuard, ")
									 T("select something to guard:"),
									 WS_CHILD | WS_VISIBLE | SS_LEFT,
									 10, 10, 400, 20, hwnd, NULL,
									 g_hInstance, NULL);

			hwcPreventShutdown = CreateWindow(
					TEXT("button"),
					TEXT("Prevent &Shutdown"),
					WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
					10, 40, 190, 20,        
					hwnd, (HMENU) OPTBTN_PREVENT_SHUTDOWN,
					g_hInstance, NULL);
			CheckDlgButton(hwnd, OPTBTN_PREVENT_SHUTDOWN, BST_CHECKED);

			hwcPreventSleep = CreateWindow(
					TEXT("button"),
					TEXT("Prevent S&leep"),
					WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
					10, 60, 190, 20,        
					hwnd, (HMENU) OPTBTN_PREVENT_SLEEP,
					g_hInstance, NULL);
			CheckDlgButton(hwnd, OPTBTN_PREVENT_SLEEP, BST_CHECKED);

			SendMessage(hwnd, WM_SETFONT, (WPARAM)g_hFont, TRUE);
			SendMessage(hwAppDesc, WM_SETFONT, (WPARAM)g_hFont, TRUE);
			SendMessage(hwcPreventShutdown, WM_SETFONT, (WPARAM)g_hFont, TRUE);
			SendMessage(hwcPreventSleep, WM_SETFONT, (WPARAM)g_hFont, TRUE);
			//InvalidateRect(hwnd, NULL, TRUE);
			
			if (IsDlgButtonChecked(hwnd, OPTBTN_PREVENT_SHUTDOWN) &&
				fShutdownBlockReasonCreate != NULL)
					fShutdownBlockReasonCreate(hwnd, T("Jon Skeet said you can't shutdown!"));
			
			break;
		case WM_DESTROY:
			if (IsDlgButtonChecked(hwnd, OPTBTN_PREVENT_SHUTDOWN) &&
				fShutdownBlockReasonDestroy != NULL)
					fShutdownBlockReasonDestroy(hwnd);
			
			PostQuitMessage(0);
			break;
		case WM_TRAY:
			switch (lParam) {
				case WM_LBUTTONDBLCLK:
					ShowWindow(hwnd, IsWindowVisible(hwnd) ? SW_HIDE : SW_SHOW);
					break;
				case WM_RBUTTONDOWN:
					ShowTrayMenu(hwnd);
					break;
			}
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case TRAYBTN_SHOW:
					ShowWindow(hwnd, IsWindowVisible(hwnd) ? SW_HIDE : SW_SHOW);
					break;
				case TRAYBTN_EXIT:
					DestroyWindow(hwnd);
					break;
				case OPTBTN_PREVENT_SHUTDOWN: {
					BOOL checked = IsDlgButtonChecked(hwnd, OPTBTN_PREVENT_SHUTDOWN);
					CheckDlgButton(hwnd, OPTBTN_PREVENT_SHUTDOWN,
							checked ? BST_UNCHECKED : BST_CHECKED);
					if (checked) {
						if (fShutdownBlockReasonCreate != NULL)
							fShutdownBlockReasonCreate(hwnd, T("Jon Skeet said you can't shutdown!"));
					} else {
						if (fShutdownBlockReasonDestroy != NULL)
							fShutdownBlockReasonDestroy(hwnd);
					}
					break;
				}
				case OPTBTN_PREVENT_SLEEP: {
					BOOL checked = IsDlgButtonChecked(hwnd, OPTBTN_PREVENT_SLEEP);
					CheckDlgButton(hwnd, OPTBTN_PREVENT_SLEEP,
							checked ? BST_UNCHECKED : BST_CHECKED);
					if (checked) {
						SetThreadExecutionState(ES_CONTINUOUS |
												ES_SYSTEM_REQUIRED |
												ES_AWAYMODE_REQUIRED |
												ES_USER_PRESENT);
					} else {
						SetThreadExecutionState(ES_CONTINUOUS);
					}
					break;
				}
			}
			break;
		case WM_QUERYENDSESSION:
			return !IsDlgButtonChecked(hwnd, OPTBTN_PREVENT_SHUTDOWN);
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}