// Andrew Han -- bathroom.c

#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include "bathroom.h"

enum state {vacant, men, women};

struct bathroomMonitor{
	struct timeval tv;
	struct timezone tz;
	pthread_mutex_t lock;
	pthread_cond_t empty;
	int occupants;
	enum state occby;
	int usages;
	int queue;
	long long timevacant;
	long long timeocc;
	long lasttime;
	long long weightedqlen;
	long lastqchange;
	long long weightedbocc;
	long lastbchange;
};

struct bathroomMonitor* overseer;

void Enter(enum gender g){
	pthread_mutex_lock(&(overseer->lock));
	if(overseer->occby == vacant){
		gettimeofday(&(overseer->tv), &(overseer->tz));
		overseer->timevacant += abs(overseer->tv.tv_usec + overseer->tv.tv_sec*1000 - overseer->lasttime);
		overseer->lasttime = overseer->tv.tv_usec + overseer->tv.tv_sec*1000;
		overseer->lastbchange = overseer->tv.tv_usec + overseer->tv.tv_sec*1000;
		overseer->occupants++;
		if(g == male)
			overseer->occby = men;
		else
			overseer->occby = women;
	}
	else if((overseer->occby == men && g == male)||(overseer->occby == women && g == female)){
		overseer->weightedbocc += abs(overseer->occupants*(overseer->tv.tv_usec + overseer->tv.tv_sec*1000 - overseer->lastbchange));
		overseer->lastbchange = overseer->tv.tv_usec + overseer->tv.tv_sec*1000;
		overseer->occupants++;
	}
	else{
		gettimeofday(&(overseer->tv), &(overseer->tz));
		overseer->weightedqlen += abs(overseer->queue*(overseer->tv.tv_usec + overseer->tv.tv_sec*1000 - overseer->lastqchange));
		overseer->lastqchange = overseer->tv.tv_usec + overseer->tv.tv_sec*1000;
		overseer->queue++;
		while((g == male && overseer->occby == women) || (g == female && overseer->occby == men)){
			pthread_cond_wait(&(overseer->empty), &(overseer->lock));
		}
		gettimeofday(&(overseer->tv), &(overseer->tz));
		overseer->weightedqlen += abs(overseer->queue*(overseer->tv.tv_usec + overseer->tv.tv_sec*1000 - overseer->lastqchange));
		overseer->lastqchange = overseer->tv.tv_usec + overseer->tv.tv_sec*1000;
		overseer->weightedbocc += abs(overseer->occupants*(overseer->tv.tv_usec + overseer->tv.tv_sec*1000 - overseer->lastbchange));
		overseer->lastbchange = overseer->tv.tv_usec + overseer->tv.tv_sec*1000;
		overseer->queue--;
		overseer->occupants++;
		if(g == male)
			overseer->occby = men;
		else
			overseer->occby = women;
	}
	pthread_mutex_unlock(&(overseer->lock));
}

void Leave(){
	pthread_mutex_lock(&(overseer->lock));
	overseer->weightedbocc += abs(overseer->occupants*(overseer->tv.tv_usec + overseer->tv.tv_sec*1000 - overseer->lastbchange));
	overseer->lastbchange = overseer->tv.tv_usec + overseer->tv.tv_sec*1000;
	overseer->occupants--;
	overseer->usages++;
	if(overseer->occupants == 0){
		overseer->occby = vacant;
		gettimeofday(&(overseer->tv), &(overseer->tz));
		overseer->timeocc += abs(overseer->tv.tv_usec + overseer->tv.tv_sec*1000 - overseer->lasttime);
		overseer->lasttime = overseer->tv.tv_usec + overseer->tv.tv_sec*1000;
		pthread_cond_broadcast(&(overseer->empty));
	}
	pthread_mutex_unlock(&(overseer->lock));
}

void Initialize(){
	overseer = malloc(sizeof(struct bathroomMonitor));
	pthread_mutex_init(&(overseer->lock), NULL);
	pthread_cond_init(&(overseer->empty), NULL);
	overseer->occupants = 0;
	overseer->occby = vacant;
	overseer->usages = 0;
	overseer->queue = 0;
	overseer->timevacant = 0;
	overseer->timeocc = 0;
	overseer->weightedqlen = 0;
	overseer->weightedbocc = 0;
	gettimeofday(&(overseer->tv), &(overseer->tz));
	overseer->lasttime = overseer->tv.tv_usec + overseer->tv.tv_sec*1000;
	overseer->lastqchange = overseer->tv.tv_usec + overseer->tv.tv_sec*1000;
	overseer->lastbchange = overseer->tv.tv_usec + overseer->tv.tv_sec*1000;
	return;
}

void Finalize(){
	printf("Bathroom usage: %d \n", overseer->usages);
	printf("Total time bathroom was vacant: %lld \n", overseer->timevacant);
	printf("Total time bathroom was occupied: %lld \n", overseer->timeocc);
	printf("Average time spent in queue: %lld \n", overseer->weightedqlen/overseer->usages);
	printf("Average occupants in bathroom: %lld \n", overseer->weightedbocc/(overseer->timevacant + overseer->timeocc));
	pthread_mutex_destroy(&(overseer->lock));
	pthread_cond_destroy(&(overseer->empty));
	free(overseer);
}


