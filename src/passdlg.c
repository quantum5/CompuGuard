#include <windows.h>
#include <stdio.h>

TCHAR szRealPassword[] = {0x6f, 0x4a, 0x4b, 0x05, 0x76, 0x4e, 0x40, 0x40, 0x51, 0x00};
#define PASSWORD_LENGTH 9

#ifdef COMPUGUARD
#include "CompuGuard.h"
#include "resource.h"

BOOL CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (msg) {
		case WM_INITDIALOG:
			g_hPassDlg = hwnd;
			break;
		case WM_COMMAND:
			switch(wParam) {
				case IDOK:
					MULTI_LINE_MACRO_BEGIN /* Abuse of Macros but OK */
						DWORD dwLength, i;
						TCHAR szPassword[PASSWORD_LENGTH+1];
						
						dwLength = GetDlgItemText(hwnd, IDC_PASSWORD, szPassword, PASSWORD_LENGTH+1);
						if (dwLength != PASSWORD_LENGTH) {
							EndDialog(hwnd, 0);
							break;
						}
						for (i = 0; i < PASSWORD_LENGTH; ++i)
							szPassword[i] ^= 37;
						EndDialog(hwnd, !lstrcmp(szPassword, szRealPassword));
						break;
					MULTI_LINE_MACRO_END;
					break;
				case IDCANCEL:
					EndDialog(hwnd, 0);
					break;
				default:
					return TRUE;
			}
			g_hPassDlg = INVALID_HANDLE_VALUE;
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
	return DialogBox(g_hInstance, MAKEINTRESOURCE(42), hwnd, DlgProc);
	//return 1;
}

#else

#include <stdio.h>
int main() {
	char buf[256];
	size_t length, i;
	printf("Hint: length is %d characters\n", PASSWORD_LENGTH);
	while (1) {
		printf("Enter string: ");
		fgets(buf, 256, stdin);
		buf[strlen(buf)-1] = 0;
		printf("You entered: %d characters\n", strlen(buf));
		if (strlen(buf) != PASSWORD_LENGTH) {
			printf("Length mismatch\n");
			continue;
		}
		for (i = 0; i < PASSWORD_LENGTH; ++i)
			buf[i] ^= 37;
		printf("%s\n", lstrcmp(buf, szRealPassword) ? "Wrong" : "Right");
	}
}
#endif
