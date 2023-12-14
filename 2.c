#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>

void exit_proccess(int signal) {
	exit(0);
}	

void infinite_reporting_loop(pid_t ppid, pid_t pid) {
	struct timeval time;
	long int hours, minutes, seconds, miliseconds;
	while(1) {
		gettimeofday(&time, NULL);
		hours = time.tv_sec/3600%24;
		minutes = time.tv_sec/60%60;
		seconds = time.tv_sec%60;
		miliseconds = time.tv_usec/1000;
		printf("This is PId=%d (parent is %d)\nReport time %ld:%ld:%ld:%ld\n\n",pid, ppid, hours, minutes, seconds, miliseconds);
	       sleep(1);	
	}
}

int main(){
	signal(SIGINT, exit_proccess);

	pid_t pid1;
	pid_t pid2;
	pid1 = fork();
	if (pid1 == -1) {
		printf("Error while creating first fork.\n");
		exit(1);
	}
	else if (pid1 == 0) {
		infinite_reporting_loop(getppid(), getpid());
	}
	else {
		pid2 = fork();
		if (pid2 == -1){
			printf("Error while creating second fork.\n");
			exit(1);
		}
		else if (pid2 == 0){
			infinite_reporting_loop(getppid(), getpid());
		}
		else{
			system("ps -x");
			infinite_reporting_loop(getppid(), getpid());
		}
	}
	return 0;
}
