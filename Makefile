SRCDIR=src
INCDIR=include

CC=cl /nologo
LD=link /nologo
CFLAGS=/c /I$(INCDIR) /W4
LDFLAGS=/subsystem:windows
RC=rc /nologo
RCFLAGS=/i$(INCDIR)
LIBS=kernel32.lib user32.lib gdi32.lib strsafe.lib shell32.lib

!IFDEF UNICODE
CFLAGS = $(CFLAGS) /DUNICODE /D_UNICODE
!ENDIF

!IFDEF DEBUG
BUILD=Debug
CFLAGS=$(CFLAGS) /Zi /DDEBUG
LDFLAGS=$(LDFLAGS) /debug
!ELSE
BUILD=Release
!ENDIF

OUTDIR=build\$(BUILD)
DISTDIR=dist\$(BUILD)
FILES=$(OUTDIR)\CompuGuard.obj \
      $(OUTDIR)\tray.obj \
      $(OUTDIR)\variables.obj \
      $(OUTDIR)\options.obj \
      $(OUTDIR)\resource.obj \
      $(OUTDIR)\CompuGuard.res

all: initdir $(DISTDIR)\CompuGuard.exe

initdir:
	if not exist build md build
	if not exist $(OUTDIR) md $(OUTDIR)
	if not exist build md dist
	if not exist $(DISTDIR) md $(DISTDIR)

$(SRCDIR)\CompuGuard.c: $(INCDIR)\CompuGuard.h
$(SRCDIR)\tray.c: $(INCDIR)\CompuGuard.h
$(SRCDIR)\variables.c: $(INCDIR)\CompuGuard.h
$(SRCDIR)\options.c: $(INCDIR)\CompuGuard.h
$(SRCDIR)\resource.c: $(INCDIR)\resource.h $(INCDIR)\CompuGuard.h
$(SRCDIR)\CompuGuard.rc: $(INCDIR)\resource.h

$(OUTDIR)\CompuGuard.res: CompuGuard.rc
	$(RC) $(RCFLAGS) /fo$@ $**

{$(SRCDIR)}.c{$(OUTDIR)}.obj::
	$(CC) $(CFLAGS) /Fo$(OUTDIR)\ $<

$(DISTDIR)\CompuGuard.exe: $(FILES)
	$(LD) /out:$@ $(LDFLAGS) $** $(LIBS)
