CC=cl /nologo
LD=link /nologo
CFLAGS=/c
LDFLAGS=/subsystem:windows /nodefaultlib /entry:WinMain
RC=rc /nologo
!IFDEF UNICODE
CFLAGS+=/DUNICODE /D_UNICODE
!ENDIF
LIBS=kernel32.lib user32.lib gdi32.lib strsafe.lib shell32.lib

all: CompuGuard.exe

CompuGuard.c: CompuGuard.h
tray.c: CompuGuard.h
variables.c: CompuGuard.h
options.c: CompuGuard.h
resource.c: resource.h CompuGuard.h
CompuGuard.rc: resource.h

CompuGuard.res: CompuGuard.rc
	$(RC) /fo$@ $**

.c.obj::
	$(CC) $(CFLAGS) $<

CompuGuard.exe: CompuGuard.obj tray.obj variables.obj options.obj resource.obj CompuGuard.res
	$(LD) /out:$@ $(LDFLAGS) $** $(LIBS)
