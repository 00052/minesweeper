all:minesweeper.exe

minesweeper.exe: sweep.h sweep.c test.c
	gcc -o $@ sweep.c test.c
clean:
	rm -f minesweeper.exe

r: minesweeper.exe
	./minesweeper.exe

