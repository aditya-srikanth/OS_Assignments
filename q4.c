#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include <string.h>
#include<sys/signal.h> 

#define BUFFER 10

void handler(int signal){
	// puts("entered parent handler");
}
void handlerChild(int signal){
	// puts("entered child handler");
}

int main(){

	int n,temp,r;
	int status;
	char k[100];
	puts("enter n k r");
	scanf("%d%d%d",&n,&temp,&r);
	temp++;
	sprintf(k,"%d",temp);
	int pid;
	int pipeKillProc[2];
	if(pipe(pipeKillProc) < 0)
		perror("can't create process");
	pid = fork();
	if(pid == 0){
		signal(SIGUSR2,handlerChild);
		int j;
		while(1){
			int i;
			for( i = 0; i < r ; i++){ 
				sleep(n);
				int Pipe[2];
				int pid1,pid2;
				if(pipe(Pipe) == -1){
					perror("pipe creation failed to execute\n");
				}
				if ((pid1 = fork()) == 0){
					close(Pipe[0]); // close read end
					dup2(Pipe[1],STDOUT_FILENO); // point output of execv to pipe
					close(Pipe[1]);
					char* arg[] = {"ps ","-eo","pid,cmd,pcpu","--sort","-pcpu",NULL};
					execv("/bin/ps",arg);
					exit(0);					
				}
				if(pid2 = fork() == 0){
					dup2(Pipe[0],STDIN_FILENO);
					close(Pipe[1]);
					close(Pipe[0]);
					char* args[] = {"head","-n",k,NULL};
					execv("/bin/head",args);
				exit(0);
				}
				// parent process i.e child 1
				waitpid(pid1,NULL,0);
				waitpid(pid2,NULL,0);
			}	
			kill(getppid(),SIGUSR1);
			sleep(60);
			close(pipeKillProc[1]);
			int processToKill;
			read(pipeKillProc[0],&processToKill,sizeof(int)+1);
			if(processToKill != -1){
				int pid3;
				pid3 = fork();
				if(pid3 == 0){
					char processToKillAsAString[BUFFER];
					// printf("processToKillAsAString %d", processToKill);
					sprintf(processToKillAsAString,"%d",processToKill);
					// printf("processToKillAsAString %s",processToKillAsAString);
					char* argsKill[] = {"kill",processToKillAsAString,NULL};
					execv("/bin/kill",argsKill);
					exit(0);
				}
				waitpid(pid3,NULL,0);
				fprintf(stderr,"exited child if\n");
				continue;
			}
	
			else{
				fprintf(stderr,"exited child else\n");
				continue;
			}
			puts("ready to repeat");
			continue;
			puts("repeating");
		}
	}
	else{
		sleep(1);
		int i;
		signal(SIGUSR1,handler);
		sleep(n*r + 10);
		while(1){
			int processToKill;
			puts("enter process to kill, -1 to do nothing");
			scanf("%d",&processToKill);
			close(pipeKillProc[0]);
			write(pipeKillProc[1],&processToKill,sizeof(int)+1);
			kill(pid,SIGUSR2);
			sleep(60);
		}
	}
	exit(0);
}

