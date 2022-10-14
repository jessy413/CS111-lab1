#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h> 
int main(int argc, char *argv[])
{
	if(argc <= 1)
	{
		perror("too few arguments\n");
		exit(EINVAL);
	}
	int prev_pipe_fd = 0;
	int exit_status = 0;
	for(int i = 1; i<argc; i++)
	{
		int pipe_fd[2];
		if(pipe(pipe_fd)==-1)
		{	
			perror("failed pipe, exit with exit status %d\n");
			exit(ESTRPIPE);
		}
		int return_code = fork();
		if(return_code==0)
		{	
			if(i!=1)
			{
				dup2(prev_pipe_fd,STDIN_FILENO);
				close(prev_pipe_fd);
			}
			if(i!=argc-1)
				dup2(pipe_fd[1],STDOUT_FILENO);
			
			close(pipe_fd[1]);
			close(pipe_fd[0]);			
			execlp(argv[i],argv[i],NULL);	
			perror("failed execution of command");
			exit(EINVAL);
		}
		else if(return_code > 0)
		{
			int pid = return_code;
			int status = 0;
			if(waitpid(pid, &status, 0)==-1)
			{
				exit(EXIT_FAILURE);
			}
			if(WIFEXITED(status))
			{
				exit_status = WEXITSTATUS(status);
				if(exit_status != 0)
					exit(exit_status);
			}
			
			if(i!=1)
			{
				close(prev_pipe_fd);
				prev_pipe_fd = pipe_fd[0];
			}
			else
				prev_pipe_fd = pipe_fd[0];
			close(pipe_fd[1]);
			if(i==argc-1)
				close(pipe_fd[0]);
	
		}
		else
		{
			exit(ECHILD);
		}
		
	}

	return 0;
	
}
