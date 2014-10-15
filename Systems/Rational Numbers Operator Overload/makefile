CFLAGS = -g -m32 -Wall

all: PA5

PA5: tester.o Rational.o
	g++ $(CFLAGS) tester.o Rational.o -o PA5
	
tester.o: Rational.h tester.cpp
	g++ $(CFLAGS) -c tester.cpp
	
Rational.o: Rational.h Rational.cpp
	g++ $(CFLAGS) -c Rational.cpp
	
clean:
	rm -f tester.o Rational.o PA5 *.exe