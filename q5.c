#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include <string.h>

 // grep -n int q4.c | cut -d ":" -f 2 if multiple files 1 if single file


int main(int argc, char const *argv[])
{
	if(argc <= 3){
		fprintf(stderr, "please enter args: newgrep PATTERN (May be in \"\")  FILE [FILE...]\n");
		exit(0);
	}
	int i;
	char* searchFiles[argc];
	char* searchQuery;
	searchQuery = malloc(sizeof(argv[1]));
	strcpy(searchQuery,argv[1]);
	int searchFilesCount = 0;
	// printf("%s\n",searchQuery );
	for(i = 2; i < argc;i++){
		char temp[100];
		strcpy(temp,argv[i]);
		// printf("%s\n",temp);
		searchFiles[i-2] = (char*)malloc(sizeof(temp));
		strcpy(searchFiles[i-2],temp);
		searchFilesCount++;
	}

	int pid1,pid2;
	int pipe1[2];
	if(pipe(pipe1) <0)
		perror("pipe1 creation error");
	if((pid1 = fork()) == 0){
		// execute grep -n searchQuery
		char* args[sizeof(searchQuery)+sizeof(searchFiles)+3];
		args[0] = "grep";
		args[1] = "-n";
		args[2] = malloc(sizeof(searchQuery));
		strcpy(args[2],searchQuery);
		for(i = 0; i < searchFilesCount; i++){
			args[3+i] = malloc(sizeof(searchFiles[i]));
			args[3+i] = searchFiles[i]; 	
		}
		args[3+searchFilesCount] = NULL;
		// for(i = 0; i < searchFilesCount + 4; i++)
		// 	printf("adf %s\n",args[i] );
		close(pipe1[0]);
		dup2(pipe1[1],STDOUT_FILENO);
		close(pipe1[1]);	
		execv("/bin/grep",args);
		perror ("execlp3"); 
		// execv("/bin/pwd",args);
		exit(0);
	}

	if((pid2 = fork()) == 0){
		waitpid(pid1,NULL,0);
		dup2(pipe1[0],STDIN_FILENO);
		close(pipe1[0]);
		close(pipe1[1]);
		if (searchFilesCount > 1){
			// printf(" > 1\n");
			close(pipe1[0]);
			char* args[] = {"cut","-d:","-f","1,2",NULL}; // printing the file name:line number for convenience
			execv("/bin/cut",args);	
			perror ("execlp1"); 
			exit(1);		
		}	
		else{
			// printf(" = 1\n");
			close(pipe1[0]);
			char* args[] = {"cut","-d:","-f","1",NULL};
			execv("/bin/cut",args);
			perror ("execlp2"); 
			exit(1);
		}
		exit(0);
	}

	close(pipe1[0]);
	close(pipe1[1]);
	waitpid(pid1,NULL,0);
	waitpid(pid2,NULL,0);
	// printf("parent khatam\n");
	return 0;
}