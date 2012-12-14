#include <windows.h>
#include <CompuGuard.h>

HHOOK hHookMouse, hHookKeyboard;

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		return 1;
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		KBDLLHOOKSTRUCT *key = (KBDLLHOOKSTRUCT*) lParam;
		
		switch (key->vkCode) {
			case VK_LWIN:
			case VK_RWIN:
			case VK_TAB:
			case VK_ESCAPE:
			case VK_LBUTTON:
			case VK_RBUTTON:
			case VK_CANCEL:
			case VK_MBUTTON:
			case VK_CLEAR:
			case VK_PAUSE:
			case VK_CAPITAL:
			case VK_KANA:
			case VK_JUNJA:
			case VK_FINAL:
			case VK_HANJA:
			case VK_NONCONVERT:
			case VK_MODECHANGE:
			case VK_ACCEPT:
			case VK_END:
			case VK_HOME:
			case VK_LEFT:
			case VK_UP:
			case VK_RIGHT:
			case VK_DOWN:
			case VK_SELECT:
			case VK_PRINT:
			case VK_EXECUTE:
			case VK_SNAPSHOT:
			case VK_INSERT:
			case VK_HELP:
			case VK_APPS:
			case VK_SLEEP:
			case VK_NUMLOCK:
			case VK_SCROLL:
			case VK_PROCESSKEY:
			case VK_ATTN:
				return 1;
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

BOOL DisableMouse(void) {
	if (hHookMouse != INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	hHookMouse = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, g_hInstance, 0);
	return TRUE;
}

BOOL DisableKeyboard(void) {
	if (hHookKeyboard != INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	hHookKeyboard = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, g_hInstance, 0);
	return TRUE;
}

BOOL EnableMouse(void) {
	if (hHookMouse == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	UnhookWindowsHookEx(hHookMouse);
	return TRUE;
}

BOOL EnableKeyboard(void) {
	if (hHookKeyboard == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	UnhookWindowsHookEx(hHookKeyboard);
	return TRUE;
}
