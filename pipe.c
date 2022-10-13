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
		printf("too few arguments, exit with exit status %d\n", EINVAL);
		exit(EINVAL);
	}
	int prev_pipe_fd = 0;
	int exit_status = 0;
	for(int i = 1; i<argc; i++)
	{
		int pipe_fd[2];
		if(pipe(pipe_fd)==-1)
		{	
			exit(ESTRPIPE);
		}
		int return_code = fork();
		if(return_code==0)
		{	
			dup2(prev_pipe_fd,STDIN_FILENO);
			if(i!=argc-1)
				dup2(pipe_fd[1],STDOUT_FILENO);
		//	close(prev_pipe_fd);	
			close(pipe_fd[1]);
			close(pipe_fd[0]);			
			execlp(argv[i],argv[i],NULL);	
			exit(EINVAL);
		}
		else if(return_code > 0)
		{
			int pid = return_code;
			int status = 0;
			waitpid(pid, &status, 0);
			if(WIFEXITED(status))
			{
				exit_status = WEXITSTATUS(status);
				if(exit_status != 0)
					exit(exit_status);
			}
			prev_pipe_fd = pipe_fd[0];
			close(pipe_fd[1]);
	
		}
		else
		{
			exit(ECHILD);
		}
		
	}

	//dup2(prev_pipe_fd,STDIN_FILENO);
	//execlp(argv[argc-1],argv[argc-1],NULL);
	//close(prev_pipe_fd);
//	printf("exit status: %d\n", exit_status);
	return 0;
	
}
