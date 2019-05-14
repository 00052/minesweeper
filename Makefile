# MinGW for windows
#CC=gcc
#RC=windres

# Winegcc
#CC=winegcc
#RC=wrc

# 32bit MinGW for linux
CC = i686-w64-mingw32-gcc
RC = i686-w64-mingw32-windres

CFLAGS = -Wall
OBJS = main.o ui.o sweep.o resource.res

all:minesweeper.exe

minesweeper.exe: $(OBJS) 
	$(CC) -mwindows -o $@ $* $^ -lgdi32 -luser32
resource.res: resource.rc resource.h
	$(RC) -O coff -o resource.res resource.rc
clean:
	$(RM) $(OBJS)
	$(RM) minesweeper.exe
	$(RM) minesweeper.exe.so #Released by Winegcc

.PHONY: all clean
