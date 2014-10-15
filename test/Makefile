all: testmailbox

testmailbox: testmailbox.o mailbox.o
	gcc -m32 -Wall -pthread -g testmailbox.o mailbox.o -o testmailbox -lm

mailbox.o: mailbox.h mailbox.c
	gcc -m32 -Wall -pthread -g -c mailbox.c -lm

testmailbox.o: mailbox.h testmailbox.c
	gcc -m32 -Wall -pthread -g -c testmailbox.c -lm

clean:
	rm -f testmailbox.o mailbox.o testmailbox *.exe
