#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main (int argc, const char * argv[]) {
	int fd[2];
        int pid, pid2;

        //Program allocates a pipe.
	pipe(fd); 

	//Parent process fork two children
	pid = fork();
        if(pid !=0)
	pid2 = fork();
        
       //Parent process prints the PID of both children on stderr 
	if(pid != 0 && pid2 != 0)
	{
		fprintf(stderr, "%s:$$ = %d\n", argv[4],pid);
		fprintf(stderr, "%s:$$ = %d\n", argv[1],pid2);
	}

        //First process' block
        if (pid2 == 0) {
                //Redirect standard output to the writing end of the pipe using dup2
                int ret = dup2(fd[1],1);
                //Error Handling
                if (ret < 0) perror("dup2");
                //First process to prepare its argument from command line arguments and executing the first command.
                char *argv2[10];
                argv2[0] = (char *)argv[1];    argv2[1] = (char *)argv[2];
                argv2[2] = NULL;
                execve(argv[1], argv2, NULL);
                return 0;
        }


        //Second process' block
	if (pid == 0) {
		close(fd[1]);
                //Redirect standard input to the reading end of the pipe using dup2
		int ret = dup2(fd[0],0);
                //Error handling
		if (ret < 0) perror("dup2");	
                //Second  process to prepare its argument from command line arguments and executing the second command, 
                //on the output of first command.
		char *argv1[10];
		argv1[0] = (char *)argv[4];  argv1[1] = (char *)argv[5];
		argv1[2] = NULL;
		execve(argv[4], argv1, NULL);
                return 0;
	} 


	// close parent's pipes
	close(fd[0]);
	close(fd[1]);
	// wait for the process to finish
	int status1,status2;
	waitpid(pid, &status2, 0);
	waitpid(pid2, &status1, 0);
	fprintf(stderr, "%s:$? = %d\n", argv[1],status1);
	fprintf(stderr, "%s:$? = %d\n", argv[4],status2);

	return 0;
}
