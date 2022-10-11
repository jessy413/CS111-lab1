#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h> 
int main(int argc, char *argv[])
{
	if(argc <= 1)
		return 0; //replace it with exit code
	int pipefd[argc];
	pipefd[0] = STDIN_FILENO;
	int prev = 0;
	for(int i = 1; i<argc; i++)
	{
	//	printf("i: %d, %d + %d", i, fd[0],fd[1]);
		int fd[2];
		if(pipe(fd)==-1)
		{
			printf("error piping");
			return 1;
		}
		int return_code = fork();
		//dup2(fd[0],STDIN_FILENO);
		//dup2(fd[1],STDOUT_FILENO);
		if(return_code==0)
		{
			printf("child %d\n", i);
			dup2(pipefd[i-1],STDIN_FILENO);
			dup2(STDOUT_FILENO,pipefd[i]);
			//printf("%d + %d\n",prev_fd,fd[1]);
			execlp(argv[i],argv[i],NULL);
			exit(0);
		}
		else if(return_code > 0)
		{
			int pid = return_code;
			int status = 0;
			//close(fd[0]);
			//close(fd[1]);
			waitpid(pid, &status, 0);
			printf("Child process exists with code: %d\n", WEXITSTATUS(status));
		}
		else
		{
			printf("Child process creation error! \n");
		}
		
	}
	return 0;
/*
	int return_code = fork();
	if(return_code == 0)
	{
		printf("child\n");
		dup2(pipefd[0],STDIN_FILENO);
		close(pipefd[0]);
		dup2(pipefd[1],STDOUT_FILENO);
		close(pipefd[1]);
		execlp(argv[1],argv[1],NULL);
		exit(0);
	}
	else if(return_code > 0)
	{
		int pid = return_code;
		int status = 0;
		waitpid(pid, &status,0);
		close(pipefd[1]);
		dup2(pipefd[0],STDIN_FILENO);
		printf("parent\n");
		execlp(argv[2],argv[2],NULL);
	}
	else
	{
		return errno;
	}
*/

	return 0;
}
