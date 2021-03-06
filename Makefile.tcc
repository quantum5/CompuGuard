SRCDIR=src
INCDIR=include

CC=tcc
CFLAGS=-I$(INCDIR) -DCOMPUGUARD -DWIN32_LEAN_AND_MEAN
LIBS=-lkernel32 -luser32 -lgdi32 -lshell32 -ladvapi32

FILES=$(SRCDIR)\CompuGuard.c \
      $(SRCDIR)\options.c \
      $(SRCDIR)\passdlg.c \
      $(SRCDIR)\pwrctrl.c \
      $(SRCDIR)\resource.c \
      $(SRCDIR)\security.c \
      $(SRCDIR)\tray.c \
      $(SRCDIR)\variables.c \

all: CompuGuard.exe

$(INCDIR)\CompuGuard.h: $(INCDIR)\qdebug.h $(INCDIR)\selfmemory.h

$(SRCDIR)\CompuGuard.c: $(INCDIR)\CompuGuard.h
$(SRCDIR)\tray.c: $(INCDIR)\CompuGuard.h
$(SRCDIR)\variables.c: $(INCDIR)\CompuGuard.h
$(SRCDIR)\options.c: $(INCDIR)\CompuGuard.h
$(SRCDIR)\resource.c: $(INCDIR)\resource.h $(INCDIR)\CompuGuard.h

CompuGuard.exe: $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES) $(LIBS)
