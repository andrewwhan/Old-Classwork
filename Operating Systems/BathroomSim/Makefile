all: bathroomSim

bathroomSim: bathroom.o bathroomSim.o
	gcc -m32 -Wall -pthread -g bathroom.o bathroomSim.o -o bathroomSim -lm
	
bathroom.o: bathroom.h bathroom.c
	gcc -m32 -Wall -pthread -g -c bathroom.c
	
bathroomSim.o: bathroom.h bathroomSim.c
	gcc -m32 -Wall -pthread -g -c bathroomSim.c -lm
	
clean:
	rm -f bathroom.o bathroomSim.o bathroomSim *.exe