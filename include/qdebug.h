#pragma once
#ifndef idD9931774_4FD2_4801_930826D0DC25DD7D
#define idD9931774_4FD2_4801_930826D0DC25DD7D

#include <stdio.h>

#define MessageErrorWndTitle(hwnd, e, t) MessageBox(hwnd, e, t, MB_ICONERROR)
#define MessageErrorWnd(hwnd, e) MessageBox(hwnd, e, T("Error!"), MB_ICONERROR)
#define MessageError(e) MessageErrorWnd(NULL, e)

#define MessageLastErrorWndTitle(hwnd, e, title) do { \
        LPTSTR s; \
        if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, e, 0, (LPTSTR)&s, 0, NULL) == 0) {\
            TCHAR str[25]; \
            snprintf(str, 25, T("%p"), str); \
            MessageErrorWndTitle(hwnd, str, title); \
        } else { \
            MessageErrorWndTitle(hwnd, s, title); \
            LocalFree(s); \
        } \
    } while (0)
#define MessageLastErrorWnd(hwnd, e) MessageLastErrorWndTitle(hwnd, e, T("Error!"))
#define MessageLastError(e) MessageLastErrorWnd(NULL, e)

#define MessageIntBox(hwnd, i, title, opt) do {\
    TCHAR buf[100];\
    snprintf(buf, 100, "%d", i);\
    MessageBox(hwnd, buf, title, opt);\
} while(0)

#define MessageIntWndTitle(hwnd, i, title) MessageIntBox(hwnd, i, title, MB_ICONINFORMATION)
#define MessageIntWnd(hwnd, i) MessageIntWndTitle(hwnd, i, T("Debug Info"))
#define MessageInt(i) MessageIntWnd(NULL, i)

#endif
