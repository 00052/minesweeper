CC=gcc
CFLAGS= -Wall

all:minesweeper.exe
minesweeper.exe: main.o ui.o sweep.o resource.res
	#gcc -o $@ $* $^ -lGdi32
	gcc -mwindows -o $@ $* $^ -lGdi32
resource.res: resource.rc resource.h
	windres -O coff -o resource.res resource.rc
clean:
	rm -f minesweeper.exe
	rm -f sweep.o
	rm -f main.o
	rm -f ui.o
	rm -f resource.res

r: minesweeper.exe
	./minesweeper.exe

