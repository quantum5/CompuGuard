SRCDIR=src
INCDIR=include

CC=cl /nologo
LD=link /nologo
CFLAGS=/c /I$(INCDIR) /W4 /Zi /DCOMPUGUARD /DWIN32_LEAN_AND_MEAN
LDFLAGS=/subsystem:windows /debug /manifest /incremental:no
LDFLAGS=$(LDFLAGS) "/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'"
RC=rc /nologo
RCFLAGS=/i$(INCDIR)
LIBS=kernel32.lib user32.lib gdi32.lib strsafe.lib shell32.lib advapi32.lib

!IFDEF UNICODE
CFLAGS = $(CFLAGS) /DUNICODE /D_UNICODE
!ENDIF

!IFDEF DEBUG
BUILD=Debug
CFLAGS=$(CFLAGS) /DDEBUG
LDFLAGS=$(LDFLAGS)
!ELSE
BUILD=Release
LDFLAGS=$(LDFLAGS)
!ENDIF

OUTDIR=build\$(BUILD)
DISTDIR=dist\$(BUILD)
FILES=$(OUTDIR)\CompuGuard.obj \
      $(OUTDIR)\options.obj \
      $(OUTDIR)\passdlg.obj \
      $(OUTDIR)\pwrctrl.obj \
      $(OUTDIR)\resource.obj \
      $(OUTDIR)\security.obj \
      $(OUTDIR)\tray.obj \
      $(OUTDIR)\variables.obj \
      $(OUTDIR)\CompuGuard.res

all: initdir $(DISTDIR)\CompuGuard.exe

initdir:
	if not exist build md build
	if not exist $(OUTDIR) md $(OUTDIR)
	if not exist build md dist
	if not exist $(DISTDIR) md $(DISTDIR)

$(INCDIR)\CompuGuard.h: $(INCDIR)\qdebug.h $(INCDIR)\selfmemory.h

$(SRCDIR)\CompuGuard.c: $(INCDIR)\CompuGuard.h
$(SRCDIR)\tray.c: $(INCDIR)\CompuGuard.h
$(SRCDIR)\variables.c: $(INCDIR)\CompuGuard.h
$(SRCDIR)\options.c: $(INCDIR)\CompuGuard.h
$(SRCDIR)\resource.c: $(INCDIR)\resource.h $(INCDIR)\CompuGuard.h
$(SRCDIR)\CompuGuard.rc: $(INCDIR)\resource.h

$(OUTDIR)\CompuGuard.res: CompuGuard.rc
	$(RC) $(RCFLAGS) /fo$@ $**

{$(SRCDIR)}.c{$(OUTDIR)}.obj::
	$(CC) $(CFLAGS) /Fo$(OUTDIR)\ /Fd$(OUTDIR)\ $<

$(DISTDIR)\CompuGuard.exe: $(FILES)
	$(LD) /out:$@ $(LDFLAGS) $** $(LIBS)
	mt.exe -nologo -manifest $@.manifest -outputresource:$@;1
	if ERRORLEVEL 0 del $@.manifest
