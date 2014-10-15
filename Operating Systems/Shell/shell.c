// Andrew Han -- shell.c

#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>

int main(int argc, char* argv[]){
	char cmdline [130];
	const char* delimiter = " \n";
	char* cmdtok [32];
	const char* cdstr = "cd";
	const char* exitstr = "exit";
	struct rusage previoususage;
	getrusage(RUSAGE_CHILDREN, &previoususage);
	while(1){
		printf("(^_^)");
		fgets(cmdline, 129, stdin);
		if(strlen(cmdline) < 128){
			if(feof(stdin)){
				printf("exit\n");
				exit(0);
			}
			puts(cmdline);
			int tokcnt = 1;
			cmdtok[0] = strtok(cmdline, delimiter);
			while(cmdtok[tokcnt-1]){
				cmdtok[tokcnt] = strtok(NULL, delimiter);
				tokcnt++;
			}
			if(tokcnt <= 32){
				if(!cmdtok[0]){

				}
				else if(!strcmp(cmdtok[0], cdstr)){
					chdir(cmdtok[1]);
				}
				else if(!strcmp(cmdtok[0], exitstr)){
					exit(0);
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
						struct rusage cumulusage;
						struct timeval starttime;
						struct timeval endtime;
						struct timezone zone;
						gettimeofday(&starttime, &zone);
						wait(&childstatus);
						gettimeofday(&endtime, &zone);
						getrusage(RUSAGE_CHILDREN, &cumulusage);
						printf("Time elapsed: %ld \n", (endtime.tv_sec - starttime.tv_sec)*1000 + (endtime.tv_usec - starttime.tv_usec)/1000);
						printf("CPU User Time: %ld \n", cumulusage.ru_utime.tv_sec*1000 + cumulusage.ru_utime.tv_usec/1000 - previoususage.ru_utime.tv_sec*1000 - previoususage.ru_utime.tv_usec/1000);
						printf("CPU System Time: %ld \n", cumulusage.ru_stime.tv_sec*1000 + cumulusage.ru_stime.tv_usec/1000 - previoususage.ru_stime.tv_sec*1000 - previoususage.ru_stime.tv_usec/1000);
						printf("Involuntary Preemptions: %ld \n", cumulusage.ru_nivcsw - previoususage.ru_nivcsw);
						printf("Voluntary Preemptions: %ld \n", cumulusage.ru_nvcsw - previoususage.ru_nvcsw);
						printf("Page faults: %ld \n", cumulusage.ru_majflt - previoususage.ru_majflt);
						printf("Page faults resolved from internal caches: %ld \n", cumulusage.ru_minflt - previoususage.ru_minflt);
						previoususage = cumulusage;
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
