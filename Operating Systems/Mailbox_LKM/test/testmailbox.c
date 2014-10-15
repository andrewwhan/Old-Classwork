// Andrew Han, Nathan Bryant -- testmailbox.c

#include "mailbox.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

struct arg{
	int* pids;
	int* replies;
	int known;
	int* toReply;
	pthread_mutex_t* replyLock;
};

void Individual(struct arg* a){	
	//printf("I'm a thread off of %d and I was just created \n", getpid());
	int toSend = rand() % 5;
	//printf("I'm a thread off of %d and I will be sending %d messages \n", getpid(), toSend);
	int sendTo;
	int sleepTime;
	int status = 0;
	int received = 0;
	int k;
	for (k = toSend; k > 0; k--){
		//printf("I know %d other processes \n", a->known);
		sendTo = rand() % a->known;
		int sendstatus = SendMsg(a->pids[sendTo], a->pids, a->known*sizeof(int), true);
		if(sendstatus == 0){
			//printf("I'm a thread off of %d and I just sent a message to %d \n", getpid(), a->pids[sendTo]);
			pthread_mutex_lock(a->replyLock);
			a->replies[*a->toReply] = a->pids[sendTo];
			(*a->toReply)++;
			pthread_mutex_unlock(a->replyLock);
		}
		else{
			//printf("Process %d no longer exists or is terminating \n", a->pids[sendTo]);
		}
		sleepTime = rand() % 2000;
		//printf("I'm a thread off of %d and I'm sleeping \n", getpid());
		usleep(sleepTime);
		//printf("I'm a thread off of %d and I just finished sleeping \n", getpid());
		while (status == 0){
			int sender;
			int len;
			int* msg = malloc(128);
			status = RcvMsg(&sender, msg, &len, false);
			if (status == 0){
				//printf("I'm a thread off of %d and I just received a message from %d \n", getpid(), sender);
				received++;
				pthread_mutex_lock(a->replyLock);
				int i;
				int flag = 0;
				int j = *a->toReply;
				for(i = 0; i < j; i++){
					if (flag == 0){
						if (a->replies[i] == sender){
							flag = 1;
							//printf("I'm a thread off of %d and I identified a reply from %d \n", getpid(), sender);
							a->replies[i] = a->replies[i+1];
							(*a->toReply)--;
						}
					}
					else{
						a->replies[i] = a->replies[i+1];
					}
				}
				pthread_mutex_unlock(a->replyLock);
				if (flag == 0){
					//printf("I'm a thread off of %d and I'm replying to %d \n", getpid(), sender);
					SendMsg(sender, a->pids, len, true);
				}
			}
			free(msg);
		}
	}
	printf("I am a thread from pid: %d\n", getpid());
	printf("Number of messages sent: %d\n", toSend);
	printf("Number of messages received: %d\n", received);
	return;
}

int spawn(int spawnwidth, int treedepth, int oldknown, int* oldpids, int avgthreads){
	treedepth--;
	int known = oldknown;
	int* pids = malloc(32*sizeof(int));
	if(oldpids != NULL){
		memcpy(pids, oldpids, 32*sizeof(int));
	}
	if(known < 32){
		pids[known] = getpid();
		known++;
	}
	else{
		int i = 0;
		for(i = 0; i<31; i++){
			pids[i] = pids[i+1];
		}
		pids[31] = getpid();
	}
	if(treedepth > 0){
		//printf("I am thread %d spawning %d \n", getpid(), spawnwidth);
		int tospawn = spawnwidth;
		while(tospawn != 0){
			tospawn--;
			int childPID = fork();
			if(childPID == 0){
				//printf("I am %d and I was just spawned \n", getpid());
				spawn(spawnwidth, treedepth, known, pids, avgthreads);
				return 0;
			}
			else{
				printf("I am %d and I just spawned %d \n", getpid(), childPID);
				if(known < 32){
					pids[known] = childPID;
					known++;
				}
				else{
					int i = 0;
					for(i = 0; i<31; i++){
						pids[i] = pids[i+1];
					}
					pids[31] = childPID;
				}
			}
		}
	}
	double a = drand48();
	double b = drand48();
	double y = sqrt(-2 * log(a)) * cos(2 * M_PI * b);
	int threads = avgthreads/2 + y * (avgthreads/2);
	if(threads == 0){
		threads = 1;
	}
	pthread_t* thread_array = malloc(threads * sizeof(pthread_t));
	int* replies = malloc(sizeof(int) * 100);
	struct arg* arguments = malloc(sizeof(struct arg));
	arguments->replies = replies;
	arguments->pids = pids;
	arguments->known = known;
	pthread_mutex_t theMutex;
	arguments->replyLock = &theMutex;
	int R = 0;
	arguments->toReply = &R;
	pthread_mutex_init(arguments->replyLock, NULL);
	int total_threads = threads;
	int j = 0;
	while (threads > 0){
		threads--;
		//printf("I'm %d and I'm creating a thread \n", getpid());
		pthread_create(&thread_array[j], NULL, (void *)&Individual, arguments);
		j++;
	}
	for (j = 0; j < total_threads; j++){
		pthread_join(thread_array[j], NULL);
	}
	while (R > 0){
		//printf("%d is still waiting on %d messages \n", getpid(), R);
		int sender;
		int len;
		int* msg = malloc(128);
		RcvMsg(&sender, msg, &len, true);
		//("Message clean-up for %d received a message from %d \n", getpid(), sender);
		int i;
		int flag = 0;
		int j = R;
		for(i = 0; i < j; i++){
			if (flag == 0){
				if (arguments->replies[i] == sender){
					flag = 1;
					//printf("Message clean-up for %d identifies a reply from %d \n", getpid(), sender);
					arguments->replies[i] = arguments->replies[i+1];
					R--;
				}
			}
			else{
				arguments->replies[i] = arguments->replies[i+1];
			}
		}
		if (flag == 0){
			SendMsg(sender, pids, len, true);
			//printf("Message clean-up for %d replies to %d \n", getpid(), sender);
		}
		free(msg);
	}
	sleep(2);
	int count;
	ManageMailbox(true, &count);
	while(count > 0){
		int sender;
		int len;
		int* msg = malloc(128);
		RcvMsg(&sender, msg, &len, true);
		SendMsg(sender, pids, len, true);
		//printf("Message clean-up for %d replies to %d \n", getpid(), sender);
		count--;
	}
	ManageMailbox(true, &count);
	//printf("There are still %d messages in %d's mailbox \n", count, getpid());
	printf("%d received all replies and is terminating, PASSED \n", getpid());
	free(replies);
	pthread_mutex_destroy(arguments->replyLock);
	free(arguments);
	free(pids);
	free(thread_array);
	return 0;
}

int main(int argc, char* argv[]){
	int spawnwidth = atoi(argv[1]);
	int treedepth = atoi(argv[2]);
	int avgthreads = atoi(argv[3]);
	int i;
	double processes;
	for(i = 0; i < treedepth; i++){
		processes = processes + pow(spawnwidth, i);
	}
	printf("This call will spawn on average %.0lf threads.\n", processes*(avgthreads+1));
	printf("I am the master thread %d \n", getpid());
	spawn(spawnwidth, treedepth, 0, NULL, avgthreads);
	return 0;
}
