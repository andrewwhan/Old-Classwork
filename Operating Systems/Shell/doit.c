// Andrew Han -- doit.c

#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>


int main(int argc, char* argv[]){
	int argshift;
	for(argshift = 0; argshift <= argc-1; argshift++){
		argv[argshift] = argv[argshift+1];
	}
	argv[argshift] = 0;
	int childpid = fork();
	if(childpid == -1){
		printf("Fork unsuccessful. \n");
	}
	else if(childpid == 0){
		int result = execvp(argv[0], argv);
		if(result == -1){
			printf("Illegal command. \n");
		}
		exit(0);
	}
	else{
		int childstatus;
		struct rusage childusage;
		struct timeval starttime;
		struct timeval endtime;
		struct timezone zone;
		gettimeofday(&starttime, &zone);
		wait(&childstatus);
		gettimeofday(&endtime, &zone);
		getrusage(RUSAGE_CHILDREN, &childusage);
		printf("Time elapsed: %ld \n", (endtime.tv_sec - starttime.tv_sec)*1000 + (endtime.tv_usec - starttime.tv_usec)/1000);
		printf("CPU User Time: %ld \n", childusage.ru_utime.tv_sec*1000 + childusage.ru_utime.tv_usec/1000);
		printf("CPU System Time: %ld \n", childusage.ru_stime.tv_sec*1000 + childusage.ru_stime.tv_usec/1000);
		printf("Involuntary Preemptions: %ld \n", childusage.ru_nivcsw);
		printf("Voluntary Preemptions: %ld \n", childusage.ru_nvcsw);
		printf("Page faults: %ld \n", childusage.ru_majflt);
		printf("Page faults resolved from internal caches: %ld \n", childusage.ru_minflt);
	}
	exit(0);
}
