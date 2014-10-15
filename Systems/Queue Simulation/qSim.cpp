/* qSim.cpp -- Andrew Han */

#include <cstdlib>
#include <iostream>
#include <ctime>
#include "Event.h"

using std::srand;
using std::rand;

int main(int argc, char* argv[]){
	if(argc <5)
		return -1;
	int customers = atoi(argv[1]);
	int tellers = atoi(argv[2]);
	float simulationTime = atof(argv[3]);
	float averageServiceTime = atof(argv[4]);
	if(argc >= 6)
		srand(atoi(argv[5]));
	else
		srand(time(NULL));

	//Single Line
	float time = 0;
	Event* queue = new Event();
	Event* singleLine = new Event();
	Stats* singleLineStats = new Stats(customers, tellers, COMMONLINE);
	for(int i=1; i<=customers; i++){
		queue = queue->InsertItem(CUSTOMERARRIVAL, simulationTime*rand()/float(RAND_MAX), i);
	}
	for(int i=1; i<=tellers; i++){
		float initialIdle = MINIDLETIME + (STARTMAXIDLETIME-MINIDLETIME)*rand()/float(RAND_MAX);
		queue = queue->InsertItem(IDLETELLER, initialIdle, i);
		singleLineStats->recordIdleTime(initialIdle);
	}
	while(queue->getType()){
		time = queue->getTime();
		queue = queue->action(singleLine, 1, averageServiceTime, singleLineStats, simulationTime);
	}
	singleLineStats->printStats(time);

	//Multi Line
	time = 0;
	Event* multiLine[tellers];
	for(int i=0; i<tellers; i++){
		multiLine[i] = new Event();
	}
	Stats* multiLineStats = new Stats(customers, tellers, MULTILINE);
	for(int i=1; i<=customers; i++){
		queue = queue->InsertItem(CUSTOMERARRIVAL, simulationTime*rand()/float(RAND_MAX), i);
	}
	for(int i=1; i<=tellers; i++){
		float initialIdle = MINIDLETIME + (STARTMAXIDLETIME-MINIDLETIME)*rand()/float(RAND_MAX);
		queue = queue->InsertItem(IDLETELLER, initialIdle, i);
		multiLineStats->recordIdleTime(initialIdle);
	}
	while(queue->getType()){
		time = queue->getTime();
		switch(queue->getType()){
		case CUSTOMERARRIVAL:
		{
			int shortest = multiLine[0]->getLen();
			int shortestLine = 0;
			for(int i = 1; i<tellers; i++){
				if(multiLine[i]->getLen() < shortest){
					shortest = multiLine[i]->getLen();
					shortestLine = i;
				}
			}
			queue = queue->action(multiLine[shortestLine], tellers, averageServiceTime, multiLineStats, simulationTime);
			break;
		}
		case IDLETELLER:
			if(multiLine[queue->getID()-1]->getLen() == 0){
				int i;
				for(i=0; i<tellers; i++){
					if(multiLine[i]->getLen())
						break;
				}
				if (i==tellers) i=0;
				queue = queue->action(multiLine[i], tellers, averageServiceTime, multiLineStats, simulationTime);
			}
			else
				queue = queue->action(multiLine[queue->getID()-1], tellers, averageServiceTime, multiLineStats, simulationTime);
			break;
		case CUSTOMERCOMPLETE:
			queue = queue->action(multiLine[0], tellers, averageServiceTime, multiLineStats, simulationTime);
			break;
		case TELLERCOMPLETE:
			if(multiLine[queue->getID()-1]->getLen() == 0){
				int i;
				for(i=0; i<=tellers-1; i++){
					if(multiLine[i]->getLen())
						break;
				}
				if(i==tellers) i=0;
				queue = queue->action(multiLine[i], tellers, averageServiceTime, multiLineStats, simulationTime);
			}
			else
				queue = queue->action(multiLine[queue->getID()-1], tellers, averageServiceTime, multiLineStats, simulationTime);
			break;
		}
	}
	multiLineStats->printStats(time);
	delete queue;
	delete singleLine;
	delete singleLineStats;
	for(int i = 0; i < tellers; i++){
		delete multiLine[i];
	}
	delete multiLineStats;
	return 0;
}
