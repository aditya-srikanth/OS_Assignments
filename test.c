#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	int pid;
	if(pid == 0){
		char* proc;
		sleep(20);
		puts("askdfja");
		sprintf(proc,"%d",getpid());
		puts("askdfja");
		execlp("/bin/kill","kill",proc,NULL);
	}
	puts("await");
	waitpid(pid,NULL,0);
	return 0;
}