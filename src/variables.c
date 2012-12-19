/* This file initialized global variables for this project */
#include "CompuGuard.h"
#include <windows.h>

HINSTANCE g_hInstance;
NOTIFYICONDATA g_nidIcon;
HWND g_hwOptions;
HANDLE g_hHeap;
HFONT g_hFont;
HBRUSH g_hBrush;
HWND g_hPassDlg;
HACCEL g_hBSODaccel;

BOOL g_BSODnoMouse;
BOOL g_BSODsecureDesk;
BOOL g_BSODnotaskmgr;
BOOL g_BSODrunning;

LPFN_SHUTDOWNBLOCKREASONCREATE fShutdownBlockReasonCreate;
LPFN_SHUTDOWNBLOCKREASONDESTROY fShutdownBlockReasonDestroy;

ACCEL BSODaccel[BSOD_ACCEL_SIZE] = {
	{FALT|FVIRTKEY, 0x31, 0xBE00},
	{FALT|FVIRTKEY, 0x33, 0xBE01},
	{FALT|FVIRTKEY, 0x35, 0xBE02},
	{FALT|FVIRTKEY, 0x37, 0xBE03},
	{FALT|FVIRTKEY, VK_F2, 0xBE04},
	{FALT|FVIRTKEY, VK_F4, 0xBE05},
	{FALT|FVIRTKEY, VK_F6, 0xBE06},
	{FALT|FVIRTKEY, VK_F8, 0xBE07},
};
BOOL BSODbAccel[BSOD_ACCEL_SIZE];

