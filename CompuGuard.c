#include <windows.h>
#include "CompuGuard.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	g_hInstance = hInstance;

	MessageBox(NULL, "Hello World!", "Hello", MB_ICONINFORMATION);
}
