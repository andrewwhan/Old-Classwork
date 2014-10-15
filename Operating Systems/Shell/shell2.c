// Andrew Han -- shell2.c

#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>

struct backgroundproc {
	int pid;
	char cmd [32];
	struct timeval begin;
};
typedef struct backgroundproc backgroundproc;

int main(int argc, char* argv[]){
	backgroundproc* running [50];
	int numberrunning = 0;
	char cmdline [130];
	const char* delimiter = " \n";
	char* cmdtok [32];
	const char* cdstr = "cd";
	const char* exitstr = "exit";
	const char* jobsstr = "jobs";
	while(1){
		printf("(^_^)");
		fgets(cmdline, 129, stdin);
		int cmdleng = strlen(cmdline);
		if(cmdleng < 128){
			if(feof(stdin)){
				printf("exit\n");
				exit(0);
			}
			puts(cmdline);
			int isbackground = 0;
			if(cmdline[cmdleng-2] == '&'){
				cmdline[cmdleng-2] = '\0';
				isbackground = 1;
			}
			int tokcnt = 1;
			cmdtok[0] = strtok(cmdline, delimiter);
			while(cmdtok[tokcnt-1]){
				cmdtok[tokcnt] = strtok(NULL, delimiter);
				tokcnt++;
			}
			if(tokcnt <= 32){
				if(!cmdtok[0]){
					//do nothing with empty command
				}
				else if(!strcmp(cmdtok[0], cdstr)){
					chdir(cmdtok[1]);
				}
				else if(!strcmp(cmdtok[0], exitstr)){
					if(numberrunning){
						printf("Allow all other processes to terminate first. \n");
					}
					else{
						exit(0);
					}
				}
				else if(!strcmp(cmdtok[0], jobsstr)){
					int i;
					for(i=0; i<numberrunning; i++){
						printf("[%d] %d %s \n", i+1, running[i]->pid, running[i]->cmd);
					}
				}
				else{
					int childpid = fork();
					if(childpid == -1){
						printf("Fork unsuccessful. \n");
					}
					else if(childpid == 0){
						int result = execvp(cmdtok[0], cmdtok);
						if(result == -1){
							printf("Illegal command. \n");
						}
						exit(0);
					}
					else{
						int childstatus;
						int waitedon;
						int latestdone = 0;
						struct rusage childusage;
						struct timeval starttime;
						struct timeval endtime;
						struct timezone zone;
						gettimeofday(&starttime, &zone);
						if(isbackground){
							backgroundproc* p = (backgroundproc*)malloc(sizeof(backgroundproc));
							p->pid = childpid;
							strcpy(p->cmd, cmdtok[0]);
							p->begin = starttime;
							running[numberrunning] = p;
							numberrunning++;
							printf("[%d] %d \n", numberrunning, p->pid);
						}
						while((waitedon = wait3(&childstatus, WNOHANG, &childusage)) && waitedon > 0){
							gettimeofday(&endtime, &zone);
							if(waitedon == childpid && !isbackground){
								latestdone = 1;
								printf("Time elapsed: %ld \n", (endtime.tv_sec - starttime.tv_sec)*1000 + (endtime.tv_usec - starttime.tv_usec)/1000);
								printf("CPU User Time: %ld \n", childusage.ru_utime.tv_sec*1000 + childusage.ru_utime.tv_usec/1000);
								printf("CPU System Time: %ld \n", childusage.ru_stime.tv_sec*1000 + childusage.ru_stime.tv_usec/1000);
								printf("Involuntary Preemptions: %ld \n", childusage.ru_nivcsw);
								printf("Voluntary Preemptions: %ld \n", childusage.ru_nvcsw);
								printf("Page faults: %ld \n", childusage.ru_majflt);
								printf("Page faults resolved from internal caches: %ld \n", childusage.ru_minflt);
								printf("\n");
							}
							else{
								int i = 0;
								while(running[i]->pid != waitedon){
									i++;
								}
								printf("[%d] %d %s completed. \n", i+1, running[i]->pid, running[i]->cmd);
								printf("Time elapsed: %ld \n", (endtime.tv_sec - running[i]->begin.tv_sec)*1000 + (endtime.tv_usec - running[i]->begin.tv_usec)/1000);
								printf("CPU User Time: %ld \n", childusage.ru_utime.tv_sec*1000 + childusage.ru_utime.tv_usec/1000);
								printf("CPU System Time: %ld \n", childusage.ru_stime.tv_sec*1000 + childusage.ru_stime.tv_usec/1000);
								printf("Involuntary Preemptions: %ld \n", childusage.ru_nivcsw);
								printf("Voluntary Preemptions: %ld \n", childusage.ru_nvcsw);
								printf("Page faults: %ld \n", childusage.ru_majflt);
								printf("Page faults resolved from internal caches: %ld \n", childusage.ru_minflt);
								printf("\n");
								free(running[i]);
								for(; i<(numberrunning-1); i++){
									running[i] = running[i+1];
								}
								numberrunning--;
							}

						}
						if(!isbackground && !latestdone){
							wait4(childpid, &childstatus, 0, &childusage);
							gettimeofday(&endtime, &zone);
							printf("Time elapsed: %ld \n", (endtime.tv_sec - starttime.tv_sec)*1000 + (endtime.tv_usec - starttime.tv_usec)/1000);
							printf("CPU User Time: %ld \n", childusage.ru_utime.tv_sec*1000 + childusage.ru_utime.tv_usec/1000);
							printf("CPU System Time: %ld \n", childusage.ru_stime.tv_sec*1000 + childusage.ru_stime.tv_usec/1000);
							printf("Involuntary Preemptions: %ld \n", childusage.ru_nivcsw);
							printf("Voluntary Preemptions: %ld \n", childusage.ru_nvcsw);
							printf("Page faults: %ld \n", childusage.ru_majflt);
							printf("Page faults resolved from internal caches: %ld \n", childusage.ru_minflt);
							printf("\n");
						}
						printf("\n");
					}
				}
			}
			else{
				printf("Please limit input to 32 arguments. \n");
			}
		}
		else{
			printf("Please limit lines to 128 characters. \n");
			char c = getc(stdin);
			while(c != '\n' && c != EOF){
				c = getc(stdin);
			}
		}
	}
	exit(0);
}
