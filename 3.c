#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <errno.h> 

int power(int base, int p) {
	if (p == 0)
		return 1;
	int result = 1;
	while(p > 0) {
		if (p % 2 == 1){
			result *= base;
		}
		base *= base;
		p /= 2;
	}
	return result;
}

void exit_proccess(int signal) {
	exit(0);
}

void proccess_infinite_reporting(int task_num, int currentPid, int parentPid) {
//printf("Process %d with parent %d has number %d\n", currentPid, parentPid, task_num);
	struct timeval time;
	long sleepTimeMS = task_num * 200;
	long long timestamp;
	while(1) {
		usleep(sleepTimeMS * 1000);
		gettimeofday(&time, NULL);
		timestamp = (((long long)time.tv_sec)*1000) + (time.tv_usec/1000);
		printf("Node %d, PID %d, PARENT PID %d, timestamp %lld\n", task_num, currentPid, parentPid, timestamp);
	}
}

void recursive_fork(int* totalProccessesStarted,  int leafsCount, int depthLeft) {
	if (depthLeft == 0)
		return;
	pid_t pid;
	for (int i = 0; i < leafsCount; i++) {
		pid = fork();
		if (pid == 0) {
			int proccessNum = *totalProccessesStarted + 1;
			int newCounter = proccessNum;
 			recursive_fork(&newCounter, leafsCount, depthLeft - 1);
			proccess_infinite_reporting(proccessNum, getpid(), getppid());
		}
		else if (pid > 0){
			if (leafsCount == 1)
				*totalProccessesStarted += depthLeft;
			else
				*totalProccessesStarted += (power(leafsCount, depthLeft) - 1) / (leafsCount - 1);
		}
	}
	for (int i = 0; i < leafsCount; i++)
		wait(NULL);

}

int main(int argc, char* argv[]) {
	signal(SIGINT, exit_proccess);
	if (argc != 3)
	{
		printf("Pass amount of leafs in node as first argument, and tree height as second.\n");
		exit(1);
	}

	int totalProccessesStarted = 0;
	int targetHeight = atoi(argv[2]);
	int eachNodeLeafsCount = atoi(argv[1]);

	if (targetHeight <= 0){
		printf("Invalid target proccesses tree height.\n");
		exit(1);
	}
	if (eachNodeLeafsCount < 0){
		printf("Invalid target node leafs count.\n");
		exit(1);
	}

	recursive_fork(&totalProccessesStarted, eachNodeLeafsCount, targetHeight);
	wait(NULL);
	return 0;
}
