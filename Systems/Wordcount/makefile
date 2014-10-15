CFLAGS = -g -m32 -Wall

all: PA3

PA3: Main.o Tree.o IO.o
	gcc $(CFLAGS) Main.o Tree.o IO.o -o PA3
	
Main.o: Tree.h IO.h Main.c
	gcc $(CFLAGS) -c Main.c
	
Tree.o: Tree.h Tree.c
	gcc $(CFLAGS) -c Tree.c

IO.o: Tree.h IO.h IO.c
	gcc $(CFLAGS) -c IO.c

clean:
	rm -f Main.o Tree.o IO.o PA3 *.exe