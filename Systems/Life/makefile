all: life

life: Life.o Board.o Game.o
	gcc -m32 -Wall Life.o Board.o Game.o -o life

Life.o: Life.h Life.c
	gcc -m32 -Wall -c Life.c

Board.o: Life.h Board.c
	gcc -m32 -Wall -c Board.c

Game.o: Life.h Game.c
	gcc -m32 -Wall -c Game.c

clean:
	rm -f Life.o Board.o Game.o life *.exe