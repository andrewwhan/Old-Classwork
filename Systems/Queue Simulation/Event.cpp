/* Event.cpp -- Andrew Han */

#include "Event.h"

Event::Event(){
	next = NULL;
	type = 0;
	time = 0;
	idnumber = 0;
}

Event::Event(int newtype, float newtime){
	next = NULL;
	type = newtype;
	time = newtime;
	idnumber = 0;
}

Event::Event(int newtype, float newtime, int newidnumber){
	next = NULL;
	type = newtype;
	time = newtime;
	idnumber = newidnumber;
}

Event::~Event(){
	delete next;
}

Event* Event::InsertItem(int newtype, float newtime, int newidnumber){
	if(!type){
		type = newtype;
		time = newtime;
		idnumber = newidnumber;
		return this;
	}
	else if(newtime < time){
		Event* p = new Event(newtype, newtime, newidnumber);
		p->next = this;
		return p;
	}
	else if(next){
		next = next->InsertItem(newtype, newtime, newidnumber);
		return this;
	}
	else{
		next = new Event(newtype, newtime, newidnumber);
		return this;
	}
}

Event* Event::RemoveItem(Event* toremove){
	if(toremove == this){
		Event* p = next;
		next = NULL;
		delete this;
		if(p)
			return p;
		else
			return new Event();
	}
	else if(next){
		next = next->RemoveItem(toremove);
		return this;
	}
	else return this;
}

float Event::getTime(){
	return time;
}

int Event::getType(){
	return type;
}

Event* Event::action(Event*& lines, int numberoflines, float averageServiceTime, Stats* statObject, float simulationTime){
	switch (type){
	case CUSTOMERARRIVAL:
		lines = lines->InsertItem(CUSTOMERINLINE, time, idnumber);
		statObject->custEnterLine(time, idnumber);
		return this->RemoveItem(this);
	case IDLETELLER:
		if(lines->type){
			float servicetime = 2*averageServiceTime*rand()/float(RAND_MAX);
			int custid = lines->idnumber;
			lines = lines->RemoveItem(lines);
			statObject->custMeetsTeller(time, custid);
			statObject->recordServiceTime(servicetime);
			return this->RemoveItem(this)->InsertItem(CUSTOMERCOMPLETE, time + servicetime, custid)->InsertItem(TELLERCOMPLETE, time + servicetime, idnumber);
		}
		else{
			if(time<simulationTime){
				float idletime = MINIDLETIME + (MAXIDLETIME-MINIDLETIME)*rand()/float(RAND_MAX);
				statObject->recordIdleTime(idletime);
				return this->RemoveItem(this)->InsertItem(IDLETELLER, time + idletime, idnumber);
			}
			else{
				return this->RemoveItem(this);
			}
		}
	case CUSTOMERCOMPLETE:
		statObject->custLeaveLine(time, idnumber);
		return this->RemoveItem(this);
	case TELLERCOMPLETE:
		if(lines->type){
			float servicetime = 2*averageServiceTime*rand()/float(RAND_MAX);
			int custid = lines->idnumber;
			lines = lines->RemoveItem(lines);
			statObject->custMeetsTeller(time, custid);
			statObject->recordServiceTime(servicetime);
			return this->RemoveItem(this)->InsertItem(CUSTOMERCOMPLETE, time + servicetime, custid)->InsertItem(TELLERCOMPLETE, time + servicetime, idnumber);
		}
		else{
			if(time<simulationTime){
				float idletime = MINIDLETIME + (MAXIDLETIME-MINIDLETIME)*rand()/float(RAND_MAX);
				statObject->recordIdleTime(idletime);
				return this->RemoveItem(this)->InsertItem(IDLETELLER, time + idletime, idnumber);
			}
			else{
				return this->RemoveItem(this);
			}
		}
	default:
		return this;
	}
}

int Event::getLen(){
	if(this && type)
		return 1 + next->getLen();
	else
		return 0;
}

int Event::getID(){
	return idnumber;
}
