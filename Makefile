all:minesweeper.exe

minesweeper.exe: sweep.h sweep.c ui.c ui.h main.c
	gcc -o $@ sweep.c ui.c main.c -lGdi32
clean:
	rm -f minesweeper.exe

r: minesweeper.exe
	./minesweeper.exe

