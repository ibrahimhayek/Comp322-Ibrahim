#include <stdio.h>
#include <time.h>
#include <sys/resource.h> 
#include <unistd.h>


/**************************************************************************/
//Fork child process doing some arbitrary work to use resources
//user time includes time needed for executing user defined code.
//System time for executing system calls (kernel time).

/**************************************************************************/

void  do_work()
{
int counter1, counter2;

for (counter1 = 0; counter1 < 100000000; counter1++) {
counter2 += counter1;
}

}


int main ()
{
  time_t time_now;
  pid_t pid, mypid, ppid;
  int status, died;

  time ( &time_now );
  printf ( "START: %ld\n", time_now );
  
  switch(pid = fork()){
 case -1:
 printf("Can't fork\n");
 return -1;

 case 0 ://return value of fork is 0, Code for child process 
  
  /* get the process id */
  if ((mypid = getpid()) < 0) {
  perror("unable to get pid");
  return -1;
  }
  /* get the parent process id */
  if ((ppid = getppid()) < 0) {
  perror("unable to get the ppid");
                           return -1;
  }
                     
  printf("PPID: %d, PID %d\n", ppid, mypid);

  do_work();

  break;

 default://return value non-zero, Code for parent process

                   //wait for the child process to finish
  died = wait(&status); 

                   /* get the process id */
                   if ((mypid = getpid()) < 0) {
                           perror("unable to get pid");
                           return -1;
                   }
                   /* get the parent process id */
                   if ((ppid = getppid()) < 0) {
                           perror("unable to get the ppid");
                           return -1;
                   }

  printf("PPID: %d, PID %d, CPID %d, RETVAL %d\n", ppid, mypid, pid, status);

  do_work();

                   //Code for extracting the user time and system time for self and the child
  struct rusage usage_self, usage_child;
                    
  getrusage (RUSAGE_SELF, &usage_self); //Parent Time
  printf ("USER: %ld.%06ld , SYS: %ld.%06ld \n", 
  (long int)usage_self.ru_utime.tv_sec, (long int)usage_self.ru_utime.tv_usec, 
  (long int)usage_self.ru_stime.tv_sec, (long int)usage_self.ru_stime.tv_usec); 

  getrusage (RUSAGE_CHILDREN, &usage_child); //Child Time
  printf ("CUSER: %ld.%06ld , CSYS: %ld.%06ld \n",
  (long int)usage_child.ru_utime.tv_sec, (long int)usage_child.ru_utime.tv_usec,
  (long int)usage_child.ru_stime.tv_sec, (long int)usage_child.ru_stime.tv_usec);


                   //Code to print the current time at the end.
  time ( &time_now );
  printf ( "STOP: %ld\n", time_now );
  }

  return 0;
}

