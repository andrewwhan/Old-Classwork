// Andrew Han -- bathroomSim.c

#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include "bathroom.h"

struct threadinfo{
	enum gender g;
	int arrival;
	int stay;
	int count;
};

void Individual(struct threadinfo* tinfo){
	enum gender g = tinfo->g;
	int arrival = tinfo->arrival;
	int stay = tinfo->stay;
	int count = tinfo->count;
	struct timeval tv;
	struct timezone tz;
	int minqueue = 0;
	int avgqueue = 0;
	int maxqueue = 0;
	int i;
	for(i = 0; i<(count); i++){
		double a = drand48();
		double b = drand48();
		double y = sqrt(-2 * log(a)) * cos(2 * M_PI * b);
		int sleepfor = arrival*y + arrival;
		if(sleepfor < 0)
			sleepfor = 0;
		usleep(sleepfor);
		gettimeofday(&tv, &tz);
		long prequeue = tv.tv_usec + tv.tv_sec;
		Enter(g);
		gettimeofday(&tv, &tz);
		long postqueue = tv.tv_usec + tv.tv_sec;
		long queuetime = postqueue - prequeue;
		if(queuetime < 0)
			queuetime = 0;
		if(minqueue == 0)
			minqueue = queuetime;
		if(queuetime < minqueue)
			minqueue = queuetime;
		if(queuetime > maxqueue)
			maxqueue = queuetime;
		avgqueue = (avgqueue*i + queuetime)/(i+1);
		a = drand48();
		b = drand48();
		y = sqrt(-2 * log(a)) * cos(2 * M_PI * b);
		sleepfor = stay*y + stay;
		if(sleepfor < 0)
			sleepfor = 0;
		usleep(sleepfor);
		Leave();
	}
	printf("Thread number: %lu \n", (unsigned long)pthread_self());
	printf("Gender: ");
	if(g == male)
		printf("Male \n");
	else
		printf("Female \n");
	printf("Number of loops: %d \n", count);
	printf("Minimum time spent in queue: %d \n", minqueue);
	printf("Average time spent in queue: %d \n", avgqueue);
	printf("Maximum time spent in queue: %d \n", maxqueue);
}

int main(int argc, char* argv[]){
	double a,b,y;
	int people = atoi(argv[1]);
	pthread_t* threads = malloc(people*sizeof(pthread_t));
	struct threadinfo* threadargs = malloc(people*sizeof(struct threadinfo));
	Initialize();
	int i;
	for(i = 0; i<people; i++){
		enum gender g;
		if(rand() % 2 == 0)
			g = male;
		else
			g = female;
		int arrival;
		int stay;
		int count;
		srand48(i);
		a = drand48();
		b = drand48();
		y = sqrt(-2 * log(a)) * cos(2 * M_PI * b);
		count = atoi(argv[2])*y + atoi(argv[2]);
		if(count < 1)
			count = -1*count;
		a = drand48();
		b = drand48();
		y = sqrt(-2 * log(a)) * cos(2 * M_PI * b);
		arrival = atoi(argv[3])*y + atoi(argv[3]);
		if(arrival < 0)
			arrival = 0;
		a = drand48();
		b = drand48();
		y = sqrt(-2 * log(a)) * cos(2 * M_PI * b);
		stay = atoi(argv[4])*y + atoi(argv[4]);
		if(stay < 0)
			stay = 0;
		threadargs[i].g = g;
		threadargs[i].arrival = arrival;
		threadargs[i].stay = stay;
		threadargs[i].count = count;
		void (*func) = &Individual;
		pthread_create(&threads[i], NULL, func, &threadargs[i]);
	}
	for(i = 0; i<people; i++){
		pthread_join(threads[i], NULL);
	}
	Finalize();
	free(threads);
	free(threadargs);
	return 0;
}
