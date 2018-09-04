#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include <string.h>

void handler(int signal){}
void handlerChild(int signal){}

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
	int pipeDup[2];
	while(1){
	if(pipe(pipeKillProc) < 0)
		perror("can't create process");
	pid = fork();
	if(pid == 0){
		pipeDup[0] = pipeKillProc[0];
		pipeDup[1] = pipeKillProc[1];
		int j;
		for( j = 0; j < 1; j++){
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
			signal(SIGUSR2,handlerChild);
			kill(getppid(),SIGUSR1);
			sleep(60);
			close(pipeKillProc[1]);
			// close(pipeKillProc[0]);
			int processToKill;
			read(pipeKillProc[0],&processToKill,sizeof(int));
			close(pipeKillProc[0]);
			// printf("proc %d\n", processToKill);
			if(processToKill != -1){
				int pid3;
				pid3 = fork();
				// printf("asdf %d\n", pid3);
				if(pid3 == 0){
					char processToKillAsAString[sizeof(int)+1];
					printf("processToKillAsAString %d", processToKill);
					sprintf(processToKillAsAString,"%d",processToKill);
					puts(processToKillAsAString);
					char* argsKill[] = {"kill",processToKillAsAString,NULL};
					// char* argsKill[] = {"",NULL};
					// kill(processToKill,SIGTERM);
					execv("/bin/kill",argsKill);
					// execv("/bin/ls",argsKill);
					exit(0);
				}
				waitpid(pid3,NULL,0);
				fprintf(stderr,"exited child if\n");
				kill(getppid(),SIGUSR1);
			}
	
			else{
				fprintf(stderr,"exited child else\n");
				kill(getppid(),SIGUSR1);
			}
		}
	}
	else{
		int i;
		pipeDup[0] = pipeKillProc[0];
		pipeDup[1] = pipeKillProc[1];
		for(i = 0; i < 1; i++){
			signal(SIGUSR1,handler);
			sleep(r*n + 10);
			// printf("WOKE UP\n");
			int processToKill;
			puts("enter process to kill, -1 to do nothing");
			scanf("%d",&processToKill);
			close(pipeKillProc[0]);
			write(pipeKillProc[1],&processToKill,sizeof(int));
			// close(pipeKillProc[1]);
			kill(pid,SIGUSR2);
			sleep(r*n + 10);
		}
			wait(NULL);
	}
	}
	exit(0);
}