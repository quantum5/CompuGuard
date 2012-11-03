#pragma once
#ifndef idD9931774_4FD2_4801_930826D0DC25DD7D
#define idD9931774_4FD2_4801_930826D0DC25DD7D

#define MessageErrorWnd(hwnd, e) MessageBox(hwnd, e, T("Error!"), MB_ICONERROR)
#define MessageError(e) MessageErrorWnd(NULL, e)

#ifdef DEBUG
#   define MessageLastErrorWnd(hwnd, e) do { \
        LPTSTR s; \
        if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, e, 0, (LPTSTR)&s, 0, NULL) == 0) {\
            TCHAR str[25]; \
            StringCchPrintf(str, 25, T("%x"), str); \
            MessageErrorWnd(hwnd, str); \
        } else { \
            MessageErrorWnd(hwnd, s); \
            LocalFree(s); \
        } \
    } while (0)
#else
#   define MessageLastErrorWnd(hwnd, e)
#endif
#define MessageLastError(e) MessageLastErrorWnd(NULL, e)

#define MessageIntBox(hwnd, i, title, opt) do {\
    TCHAR buf[100];\
    StringCchPrintf(buf, 100, "%d", i);\
    MessageBox(hwnd, buf, title, opt);\
} while(0)

#define MessageIntWndTitle(hwnd, i, title) MessageIntBox(hwnd, i, title, MB_ICONINFORMATION)
#define MessageIntWnd(hwnd, i) MessageIntWndTitle(hwnd, i, T("Debug Info"))
#define MessageInt(i) MessageIntWnd(NULL, i)

#endif
