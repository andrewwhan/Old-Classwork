/* Event.h -- Andrew Han */

#include <cstdlib>
#include "Stats.h"

int const CUSTOMERARRIVAL = 1;
int const IDLETELLER = 2;
int const CUSTOMERCOMPLETE = 3;
int const TELLERCOMPLETE = 4;
int const CUSTOMERINLINE = 5;


float const MINIDLETIME = 1.0/60.0;
float const MAXIDLETIME = 2.5; // Maximum idle time when teller lines are empty
float const STARTMAXIDLETIME = 10.0; // Maximum idle time at beginning of day

class Event{
public:
	Event* InsertItem(int newtype, float newtime, int newidnumber);
	Event* RemoveItem(Event* toremove);
	Event();
	Event(int newtype, float newtime);
	Event(int newtype, float newtime, int newidnumber);
	~Event();
	Event* action(Event*& lines, int numberoflines, float averageServiceTime, Stats* statObject, float simulationTime);
	float getTime();
	int getType();
	int getLen();
	int getID();
private:
	int type;
	float time;
	Event* next;
	int idnumber;
};
