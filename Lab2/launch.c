#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t child_pid;
	int child_status;
	child_pid = fork();

	if(child_pid == 0) {
		/* This is done by the child process. */
		char *env_args[] = { NULL };
		execve((char *)argv[1],&argv[1], env_args);
		return 0;
	}
	else {
		/* This is run by the parent.  Wait for the child
		   to terminate. */

			fprintf(stderr, "%s:$$ = %d\n", argv[1],child_pid);
			pid_t tpid = wait(&child_status);
			fprintf(stderr, "%s:$? = %d\n", argv[1],child_status);
	}
	return 0;
}

