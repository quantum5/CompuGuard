#include <windows.h>
#include <resource.h>
#include <CompuGuard.h>

HBITMAP hbBSOD;
LRESULT CALLBACK BSODProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

void RegisterBSODWindowClass(void) {
	WNDCLASSEX wincl;
	
	wincl.hInstance = g_hInstance;
	wincl.lpszClassName = BSOD_WIN_CLASS;
	wincl.lpfnWndProc = BSODProc;
	wincl.style = CS_DBLCLKS;
	wincl.cbSize = sizeof(WNDCLASSEX);
	wincl.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(RID_LOCK));
	wincl.hIconSm = LoadIcon(NULL, MAKEINTRESOURCE(RID_LOCK));
	wincl.hCursor = NULL;
	wincl.lpszMenuName = NULL;
	wincl.cbClsExtra = 0;
	wincl.cbWndExtra = 0;
	wincl.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	
	if (!RegisterClassEx(&wincl)) {
		MessageError("Can't register BSOD class!");
		ExitProcess(1);
	}
}

void InitializeBSOD(void) {
	hbBSOD = LoadBitmap(g_hInstance, MAKEINTRESOURCE(RID_BSOD));
	RegisterBSODWindowClass();
}

void ShowBSOD(void) {
	if (g_BSODsecureDesk)
		EnterSecureDesk();
	MessageBox(NULL, "Not Really Implemented", NULL, 0);
}

void HideBSOD(void) {
	if (g_BSODsecureDesk)
		SetDesktop(g_hOldDesk);
}

LRESULT CALLBACK BSODProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	int i;
	switch (message) {
		case WM_CREATE:
			// Bitmap holder
			scwnd = CreateWindowEx(0, "STATIC", "",
					/*SS_REALSIZECONTROL | */SS_BITMAP | WS_CHILD | WS_VISIBLE,
					0, 0, 640, 480,
					hwnd, (HMENU)-1, NULL, NULL);
			SetTimer(hwnd, 0xBEEF, 1000, NULL);
			SetTimer(hwnd, 0xFAC, 1000, NULL);

			// Hide cursor
			SetCursor(NULL);

			// Block shutdown
			if (fShutdownBlockReasonCreate != NULL)
				fShutdownBlockReasonCreate(hwnd, L"You can't shutdown with a BSoD running.");

			// Shortcuts
			hAccel = CreateAcceleratorTable((LPACCEL)&accel, ACCEL_SIZE+1);

			// Force to front
			SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_ASYNCWINDOWPOS | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
			SetForegroundWindow(hwnd);
			LockSetForegroundWindow(1);
			AllowAccessibilityShortcutKeys(FALSE);

			if (g_BSODnotaskmgr)
				DisableTaskManager();
			break;
		case WM_SHOWWINDOW:
			break;
		case WM_TIMER:
			if (wParam == 0xBEEF) {
				KillTimer(hwnd, 0xBEEF);
				SendMessage(scwnd, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);
				GetClientRect(hwnd, &rectWinSize);
				SetWindowPos(scwnd, 0, 0, 0, rectWinSize.right,
								rectWinSize.bottom, SWP_NOMOVE |
								SWP_NOZORDER | SWP_NOACTIVATE);
				// Repaint the Window so the smaller one doesn't cover the bigger one
				InvalidateRect(hwnd, NULL, TRUE);
			} else if (wParam == 0xDEAD) {
				KillTimer(hwnd, 0xDEAD);
				DestroyWindow(hwnd);
			} else if (wParam = 0xFAC) {
				SetDesktop(hNewDesk);
			}
			break;
		case WM_DESTROY:
			if (fShutdownBlockReasonDestroy != NULL)
				fShutdownBlockReasonDestroy(hwnd);

			// Low Level Hooks
			UnhookWindowsHookEx(hhkKeyboard);
			if (g_BSODnoMouse)
				UnhookWindowsHookEx(hhkMouse);

			// Clean up resources
			DestroyAcceleratorTable(hAccel);
			LockSetForegroundWindow(0);
			SelfFree(szDeskName);
			AllowAccessibilityShortcutKeys(TRUE);
			
			if (g_BSODsecureDesk) {
				// Out of Secure Desktop
				SetDesktop(hOldDesk);
				CloseDesktop(hNewDesk);
			}
			if (g_BSODnotaskmgr)
				EnableTaskManager();
			// Good bye
			PostQuitMessage(0);
			break;
		case WM_CLOSE:
			switch (DialogBox(hInst, MAKEINTRESOURCE(32), hwnd, DlgProc)) {
				case 1:
					DestroyWindow(hwnd);
					break;
				case 2:
					hdlg = INVALID_HANDLE_VALUE;
					MessageBox(hwnd, "You got the password wrong!\n"
								"Good luck guessing!", "Error!", MB_ICONERROR);
					break;
				default:
					hdlg = INVALID_HANDLE_VALUE;
					MessageBox(hwnd, "You just abandoned the perfect chance to exit!\n"
								"Good luck trying!", "Error!", MB_ICONERROR);
			}
			break;
		case WM_KEYDOWN: // any key without ALT, NYET!
			return 0;
		case WM_COMMAND:
		case WM_SYSCOMMAND:
			// Only perform work when it's a register key combination
			// else just ignore
			if (HIWORD(wParam) == 1) {
				if (LOWORD(wParam) == 0xDEAD) {
					for (i = 0; i < ACCEL_SIZE; ++i)
						if (!bAccel[i])
							return 0;
					SendMessage(hwnd, WM_CLOSE, 0, 0);
				} else if ((LOWORD(wParam) & 0xFF00) == 0xBE00) {
					bAccel[LOWORD(wParam) & 0xFF] = TRUE;
				}
			}
			break;
		case WM_QUERYENDSESSION: // Umm, tryink to logoff? NYET!
			break;
		case WM_ENFORCE_FOCUS: // Call this when I lose focus
			if (GetForegroundWindow() != hdlg) {
				printf("hwnd: %d\n", hwnd);
				if (hdlg != INVALID_HANDLE_VALUE) {
					SetFocus(hdlg);
					SetForegroundWindow(hdlg);
				} else {
					SetFocus(hwnd);
					SetForegroundWindow(hwnd);
				}
			}
			ShowWindow(hwnd, SW_SHOWMAXIMIZED); // Force maximize
			break;
		case WM_ACTIVATE:
			if (LOWORD(wParam) != WA_INACTIVE)
				break;
			if (!HIWORD(wParam))
				break;
		case WM_NCACTIVATE: // Above are cases that focus can be gain or lost
		case WM_KILLFOCUS: // All the cases that focus is lost
			PostMessage(hwnd, WM_ENFORCE_FOCUS, 0, 0);
			return 1;
		case WM_SIZE:
			if (wParam != SIZE_MAXIMIZED) // So you are not maximizing? Then WTF are you doing?
				ShowWindow(hwnd, SW_SHOWMAXIMIZED); // Force maximization
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
