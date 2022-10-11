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
	/*
	int start_return_code = fork();
	if(start_return_code==0)
	{
		dup2(fd[1], STDOUT_FILENO);
		execlp(argv[1],argv[1],NULL);
		exit(0);
	}
	else if(start_return_code > 0)
	{
		int pid = start_return_code;
		int status = 0;
		waitpid(pid, &status, 0);

	}
	else
	{
		printf("Error creating child starting");
	}
	
	*/
	int prev_pipe_fd = 0;
	for(int i = 1; i<argc; i++)
	{
		int pipe_fd[2];
		pipe(pipe_fd);
		int return_code = fork();
		if(return_code==0)
		{	
			dup2(prev_pipe_fd,STDIN_FILENO);
			printf("child: %d + %d\n", prev_pipe_fd,STDIN_FILENO);
			dup2(pipe_fd[1],STDOUT_FILENO);
			close(prev_pipe_fd);			
			execlp(argv[i],argv[i],NULL);
			exit(0);
		}
		else if(return_code > 0)
		{
			int pid = return_code;
			int status = 0;
			waitpid(pid, &status, 0);
			prev_pipe_fd = 3;
			close(pipe_fd[1]);
			printf("%d + %d", pipe_fd[0],prev_pipe_fd);
			printf("Child process exists with code: %d\n", WEXITSTATUS(status));
		}
		else
		{
			printf("Child process creation error! \n");
		}
		
	}
	dup2(prev_pipe_fd,STDOUT_FILENO);
	close(prev_pipe_fd);
/*
	dup2(fd[0],STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	execlp(argv[argc-1],argv[argc-1],NULL);
*/	return 0;
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

}
