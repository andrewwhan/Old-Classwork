/* Stats.h -- Andrew Han */

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <iomanip>

int const COMMONLINE = 0;
int const MULTILINE = 1;

class Stats{
public:
	void custEnterLine(float time, int customerid);
	void custMeetsTeller(float time, int customerid);
	void custLeaveLine(float time, int customerid);
	void recordIdleTime(float timespent);
	void recordServiceTime(float timespent);
	void printStats(float timecompleted);
	float averageTimeSpent();
	float standardDeviation();
	Stats(int customers, int tellers, int queue);
	~Stats();
private:
	int customersServed;
	float* customerData;
	int numberOfTellers;
	int queueType;
	float tellerServiceTime;
	float tellerIdleTime;
	float maxLineTime;
};
