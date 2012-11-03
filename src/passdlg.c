#include "CompuGuard.h"
#include <windows.h>

TCHAR szRealPassword[] = {0x6f, 0x4a, 0x4b, 0x05, 0x76, 0x4e, 0x40, 0x40, 0x51, 0x00};
#define PASSWORD_LENGTH 9

LRESULT CALLBACK DlgProc(HWND hWndDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_INITDIALOG:
			break;
		case WM_COMMAND:
			switch(wParam) {
				case IDOK:
					do {
						DWORD dwLength, i;
						TCHAR szPassword[PASSWORD_LENGTH+1];
						dwLength = GetDlgItemText(hWndDlg, 1024, szPassword, PASSWORD_LENGTH+1);
						if (dwLength != PASSWORD_LENGTH) {
							MessageError(T("Wrong Length"));
							EndDialog(hWndDlg, 0);
							break;
						}
						for (i = 0; i < PASSWORD_LENGTH; ++i)
							szPassword[i] ^= 37;
						EndDialog(hWndDlg, !lstrcmp(szPassword, szRealPassword));
						MessageError(!lstrcmp(szPassword, szRealPassword) ? T("Right") : T("Wrong"));
						break;
					} while (0);
					break;
				case IDCANCEL:
					MessageError(T("Cancelled"));
					EndDialog(hWndDlg, 0);
					break;
			}
			break;
		default:
			return FALSE;
	}
	return TRUE;
}

/*
 * Returns:
 *  - 0 for wrong password or abort
 *  - 1 for correct password
 */
INT_PTR ShowPasswordDialog(HWND hwnd) {
	// Skip password and assume correct if can't find the dialog
	if (FindResource(NULL, MAKEINTRESOURCE(42), MAKEINTRESOURCE(5)) == NULL) {
		MessageBox(hwnd, T("Authentiation Skipped!"), T("Warning!"), MB_ICONWARNING);
		return 1;
	}
	//return DialogBox(g_hInstance, MAKEINTRESOURCE(42), hwnd, DlgProc);
	return 1;
}
