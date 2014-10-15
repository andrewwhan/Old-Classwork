all: doit shell shell2

doit: doit.c
	gcc doit.c -m32 -Wall -g -o doit
	
shell: shell.c
	gcc shell.c -m32 -Wall -g -o shell

shell2: shell2.c
	gcc shell2.c -m32 -Wall -g -o shell2

clean:
	rm -f doit shell shell2 *.exe