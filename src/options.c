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
	wincl.hbrBackground = g_hBrush;

	if(!RegisterClassEx(&wincl)) {
		MessageError(T("Can't register windows class '") OPTION_WIN_CLASS T("'!"));
		ExitProcess(0);
	}

	hwnd = CreateWindowEx(0, OPTION_WIN_CLASS, T("Computer Guard Options"),
			WS_CAPTION | WS_SYSMENU | WS_BORDER, CW_USEDEFAULT,
			CW_USEDEFAULT, 420, 250,
			NULL, NULL, g_hInstance, NULL);

	UpdateWindow(hwnd);
	g_hwOptions = hwnd;
}

LRESULT CALLBACK OptionsWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static HWND hwAppDesc, hwcPreventShutdown, hwcPreventSleep,
				hwgbBSOD, hwcBSODnoMouse, hwcBSODsecureDesk,
				hwcBSODnoTaskmgr, hwbBSOD;
	switch (message) {
		case WM_CREATE:
			hwAppDesc = CreateWindow(T("STATIC"),
					T("Welcome to CompuGuard, ")
					T("select something to guard:"),
					WS_CHILD | WS_VISIBLE | SS_LEFT,
					10, 10, 400, 20, hwnd, NULL,
					g_hInstance, NULL);

			hwcPreventShutdown = CreateWindow(TEXT("button"),
					TEXT("Prevent &Shutdown"),
					WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
					10, 40, 190, 20,
					hwnd, (HMENU) OPTBTN_PREVENT_SHUTDOWN,
					g_hInstance, NULL);
			CheckDlgButton(hwnd, OPTBTN_PREVENT_SHUTDOWN, BST_CHECKED);

			hwcPreventSleep = CreateWindow(TEXT("button"),
					TEXT("Prevent S&leep"),
					WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
					10, 60, 190, 20,
					hwnd, (HMENU) OPTBTN_PREVENT_SLEEP,
					g_hInstance, NULL);
			CheckDlgButton(hwnd, OPTBTN_PREVENT_SLEEP, BST_CHECKED);

			// Blue Screen Section
			hwgbBSOD = CreateWindow(TEXT("button"),
					TEXT("&Blue Screen of Death"),
					WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
					210, 40, 190, 100, hwnd, NULL,
					g_hInstance, NULL);

			hwcBSODnoMouse = CreateWindow(
					TEXT("button"),
					TEXT("Disable &Mouse"),
					WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
					220, 55, 150, 20,
					hwnd, (HMENU) OPTBTN_BSOD_DISABLE_MOUSE,
					g_hInstance, NULL);
			CheckDlgButton(hwnd, OPTBTN_BSOD_DISABLE_MOUSE, BST_CHECKED);

			hwcBSODsecureDesk = CreateWindow(
					TEXT("button"),
					TEXT("Use &Secure Desktop"),
					WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
					220, 75, 150, 20,
					hwnd, (HMENU) OPTBTN_BSOD_SECURE_DESK,
					g_hInstance, NULL);
			CheckDlgButton(hwnd, OPTBTN_BSOD_SECURE_DESK, BST_CHECKED);

			hwcBSODnoTaskmgr = CreateWindow(
					TEXT("button"),
					TEXT("Disable &Task Manager"),
					WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
					220, 75, 150, 20,
					hwnd, (HMENU) OPTBTN_BSOD_NO_TASKMGR,
					g_hInstance, NULL);
			CheckDlgButton(hwnd, OPTBTN_BSOD_NO_TASKMGR, BST_CHECKED);

			hwbBSOD = CreateWindow(TEXT("button"),
					TEXT("Show &BSOD"),
					WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
					220, 95, 170, 20,
					hwnd, (HMENU) OPTBTN_BSOD_RUN,
					g_hInstance, NULL);

			SendMessage(hwnd, WM_SETFONT, (WPARAM) g_hFont, TRUE);
			SendMessage(hwAppDesc, WM_SETFONT, (WPARAM) g_hFont, TRUE);
			SendMessage(hwcPreventShutdown, WM_SETFONT, (WPARAM) g_hFont, TRUE);
			SendMessage(hwcPreventSleep, WM_SETFONT, (WPARAM) g_hFont, TRUE);
			SendMessage(hwgbBSOD, WM_SETFONT, (WPARAM) g_hFont, TRUE);
			SendMessage(hwcBSODnoMouse, WM_SETFONT, (WPARAM) g_hFont, TRUE);
			SendMessage(hwcBSODsecureDesk, WM_SETFONT, (WPARAM) g_hFont, TRUE);
			SendMessage(hwbBSOD, WM_SETFONT, (WPARAM) g_hFont, TRUE);
			
			if (IsDlgButtonChecked(hwnd, OPTBTN_PREVENT_SHUTDOWN) &&
				fShutdownBlockReasonCreate != NULL)
					fShutdownBlockReasonCreate(hwnd, T("Jon Skeet said you can't shutdown!"));
			
			break;
		case WM_EXIT:
			if (ShowPasswordDialog(hwnd))
				DestroyWindow(hwnd);
			else
				MessageBox(hwnd, T("Wrong password!"), T("Error"), MB_ICONERROR);
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
					PostMessage(hwnd, WM_EXIT, 0, 0);
					break;
				case OPTBTN_PREVENT_SHUTDOWN: {
					BOOL checked = IsDlgButtonChecked(hwnd, OPTBTN_PREVENT_SHUTDOWN);
					CheckDlgButton(hwnd, OPTBTN_PREVENT_SHUTDOWN,
							checked ? BST_UNCHECKED : BST_CHECKED);
					PreventShutdown(hwnd, checked);
					break;
				}
				case OPTBTN_PREVENT_SLEEP: {
					BOOL checked = IsDlgButtonChecked(hwnd, OPTBTN_PREVENT_SLEEP);
					CheckDlgButton(hwnd, OPTBTN_PREVENT_SLEEP,
							checked ? BST_UNCHECKED : BST_CHECKED);
					PreventSleep(checked);
					break;
				}
				case OPTBTN_BSOD_DISABLE_MOUSE: {
					g_BSODnoMouse = IsDlgButtonChecked(hwnd, OPTBTN_BSOD_DISABLE_MOUSE);
					CheckDlgButton(hwnd, OPTBTN_BSOD_DISABLE_MOUSE,
							g_BSODnoMouse ? BST_UNCHECKED : BST_CHECKED);
					break;
				}
				case OPTBTN_BSOD_SECURE_DESK: {
					g_BSODsecureDesk = IsDlgButtonChecked(hwnd, OPTBTN_BSOD_SECURE_DESK);
					CheckDlgButton(hwnd, OPTBTN_BSOD_SECURE_DESK,
							g_BSODsecureDesk ? BST_UNCHECKED : BST_CHECKED);
					break;
				}
				case OPTBTN_BSOD_NO_TASKMGR: {
					g_BSODnotaskmgr = IsDlgButtonChecked(hwnd, OPTBTN_BSOD_NO_TASKMGR);
					CheckDlgButton(hwnd, OPTBTN_BSOD_NO_TASKMGR,
							g_BSODnotaskmgr ? BST_UNCHECKED : BST_CHECKED);
					break;
				}
				case OPTBTN_BSOD_RUN:
					ShowBSOD();
					break;
			}
			break;
		case WM_CLOSE:
			ShowWindow(hwnd, SW_HIDE);
			return 0;
		case WM_QUERYENDSESSION:
			return !IsDlgButtonChecked(hwnd, OPTBTN_PREVENT_SHUTDOWN);
		case WM_CTLCOLORSTATIC: {
			HDC hdcStatic = (HDC) wParam;
			SetBkColor(hdcStatic, RGB(0xF0, 0xF0, 0xF0));
			return (INT_PTR) g_hBrush;
		}
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}