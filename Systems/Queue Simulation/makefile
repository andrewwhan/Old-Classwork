CFLAGS = -g -m32 -Wall

all: qSim

qSim: qSim.o Event.o Stats.o
	g++ $(CFLAGS) qSim.o Event.o Stats.o -o qSim
	
qSim.o: Event.h Stats.h qSim.cpp
	g++ $(CFLAGS) -c qSim.cpp
	
Event.o: Event.h Stats.h Event.cpp
	g++ $(CFLAGS) -c Event.cpp
	
Stats.o: Stats.h Stats.cpp
	g++ $(CFLAGS) -c Stats.cpp
	
clean:
	rm -f qSim.o Event.o Stats.o qSim *.exe