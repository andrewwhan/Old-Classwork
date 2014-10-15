/* Stats.cpp -- Andrew Han */

#include "Stats.h"
using std::cout;
using std::endl;
using std::setprecision;

Stats::Stats(int customers, int tellers, int queue){
	customersServed = 0;
	customerData = new float[customers];
	numberOfTellers = tellers;
	queueType = queue;
	tellerServiceTime = 0;
	tellerIdleTime = 0;
	maxLineTime = 0;
}

Stats::~Stats(){
	delete[] customerData;
}

void Stats::custEnterLine(float time, int customerid){
	customerData[customerid-1] = time;
}

void Stats::custMeetsTeller(float time, int customerid){
	if(time - customerData[customerid-1] > maxLineTime)
		maxLineTime = time - customerData[customerid-1];
}

void Stats::custLeaveLine(float time, int customerid){
	customerData[customerid-1] = time-customerData[customerid-1];
	customersServed++;
}

void Stats::recordIdleTime(float timespent){
	tellerIdleTime += timespent;
}

void Stats::recordServiceTime(float timespent){
	tellerServiceTime += timespent;
}

float Stats::averageTimeSpent(){
	float total = 0;
	for(int i = 0; i<customersServed; i++){
		total += customerData[i];
	}
	return total/float(customersServed);
}

float Stats::standardDeviation(){
	float mean = averageTimeSpent();
	float sumofsquares = 0;
	for(int i = 0; i<customersServed; i++){
		sumofsquares += pow(customerData[i]-mean, 2);
	}
	return sqrt(sumofsquares/customersServed);
}

void Stats::printStats(float timecompleted){
	if(queueType == COMMONLINE)
		cout<<"One common line was used."<<endl;
	else
		cout<<"A line for each teller was used."<<endl;
	cout<<"Customers served: "<<customersServed<<endl;
	cout<<"Time to serve all customers: "<<setprecision(4)<<timecompleted<<" minutes"<<endl;
	cout<<"Number of tellers: "<<numberOfTellers<<endl;
	cout<<"Average time spent in bank: "<<setprecision(4)<<averageTimeSpent()<<" minutes"<<endl;
	cout<<"Standard deviation: "<<setprecision(4)<<standardDeviation()<<" minutes"<<endl;
	cout<<"Maximum line wait time: "<<setprecision(4)<<maxLineTime<<" minutes"<<endl;
	cout<<"Total teller service time: "<<setprecision(4)<<tellerServiceTime<<" minutes"<<endl;
	cout<<"Total teller idle time: "<<setprecision(4)<<tellerIdleTime<<" minutes"<<endl;
}
