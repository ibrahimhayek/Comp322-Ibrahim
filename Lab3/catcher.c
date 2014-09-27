#include <signal.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

static int total_signal_count=0;
static int term_signal_count=0;

/* The code below maps the name of a signal to its corresponding number.      */
int sig_name2number(char *s) 
{
  int signumber = -1;

  if (!strcmp(s, "HUP"))     { signumber = SIGHUP; }
  if (!strcmp(s, "INT"))     { signumber = SIGINT; }
  if (!strcmp(s, "QUIT"))    { signumber = SIGQUIT; }
  if (!strcmp(s, "ILL"))     { signumber = SIGILL; }
  if (!strcmp(s, "TRAP"))    { signumber = SIGTRAP; }
  if (!strcmp(s, "ABRT"))    { signumber = SIGABRT; }
  if (!strcmp(s, "IOT"))     { signumber = SIGIOT; }
  if (!strcmp(s, "BUS"))     { signumber = SIGBUS; }
  if (!strcmp(s, "FPE"))     { signumber = SIGFPE; }
  if (!strcmp(s, "KILL"))    { signumber = SIGKILL; }
  if (!strcmp(s, "USR1"))   { signumber = SIGUSR1 ; }
  if (!strcmp(s, "SEGV"))   { signumber = SIGSEGV ; }
  if (!strcmp(s, "USR2"))   { signumber = SIGUSR2 ; }
  if (!strcmp(s, "PIPE"))   { signumber = SIGPIPE ; }
  if (!strcmp(s, "ALRM"))   { signumber = SIGALRM ; }
  if (!strcmp(s, "TERM"))   { signumber = SIGTERM ; }
  if (!strcmp(s, "STKFLT")) { signumber = SIGSTKFLT ; }
  if (!strcmp(s, "CHLD"))   { signumber = SIGCHLD ; }
  if (!strcmp(s, "CONT"))   { signumber = SIGCONT ; }
  if (!strcmp(s, "STOP"))   { signumber = SIGSTOP ; }
  if (!strcmp(s, "TSTP"))   { signumber = SIGTSTP ; }
  if (!strcmp(s, "TTIN"))   { signumber = SIGTTIN ; }
  if (!strcmp(s, "TTOU"))   { signumber = SIGTTOU ; }
  if (!strcmp(s, "URG"))    { signumber = SIGURG ; }
  if (!strcmp(s, "XCPU"))   { signumber = SIGXCPU ; }
  if (!strcmp(s, "XFSZ"))   { signumber = SIGXFSZ ; }
  if (!strcmp(s, "VTALRM")) { signumber = SIGVTALRM ; }
  if (!strcmp(s, "PROF"))   { signumber = SIGPROF ; }
  if (!strcmp(s, "WINCH"))  { signumber = SIGWINCH ; }

  return signumber;
}

/* The code below maps the signal number to its corresponding signal name.      */
char * sig_number2name(int sig)
{
char *s = NULL;

if (sig == SIGHUP) s = strdup("HUP");
if (sig == SIGINT) s = strdup("INT");
if (sig == SIGQUIT) s = strdup("QUIT");
if (sig == SIGILL) s = strdup("ILL");
if (sig == SIGTRAP) s = strdup("TRAP");
if (sig == SIGABRT) s = strdup("ABRT");
if (sig == SIGIOT) s = strdup("IOT");
if (sig == SIGBUS) s = strdup("BUS");
if (sig == SIGFPE) s = strdup("FPE");
if (sig == SIGKILL) s = strdup("KILL");
if (sig == SIGUSR1) s = strdup("USR1");
if (sig == SIGSEGV) s = strdup("SEGV");
if (sig == SIGUSR2) s = strdup("USR2");
if (sig == SIGPIPE) s = strdup("PIPE");
if (sig == SIGALRM) s = strdup("ALRM");
if (sig == SIGTERM) s = strdup("TERM");
if (sig == SIGSTKFLT) s = strdup("STKFLT");
if (sig == SIGCHLD) s = strdup("CHLD");
if (sig == SIGCONT) s = strdup("CONT");
if (sig == SIGSTOP) s = strdup("STOP");
if (sig == SIGTSTP) s = strdup("TSTP");
if (sig == SIGTTIN) s = strdup("TTIN");
if (sig == SIGTTOU) s = strdup("TTOU");
if (sig == SIGURG) s = strdup("URG");
if (sig == SIGXCPU) s = strdup("XCPU");
if (sig == SIGXFSZ) s = strdup("XFSZ");
if (sig == SIGVTALRM) s = strdup("VTALRM");
if (sig == SIGPROF) s = strdup("PROF");
if (sig == SIGWINCH) s = strdup("WINCH");

return s;
}

//Common Signal Handler for all the registered Signals.
void sig_handler(int signo)
{
	time_t time_now;
	time ( &time_now );

        signal(signo, sig_handler);
        
	printf("%s caught %ld\n ",sig_number2name(signo),time_now);

	total_signal_count++;

	if(signo == SIGTERM)
		term_signal_count++;
	else
		term_signal_count=0;
}

int main(int argc, char *argv[])
{
	pid_t mypid;
	int counter;

	/* get the process id */
	if ((mypid = getpid()) < 0) {
		perror("unable to get pid");
		return -1;
	}
	fprintf(stderr, "%s:$$ = %d\n", argv[0],mypid);  

        //Register the Signal for all the command line arguments processes.
	for (counter=1;counter<=(argc-1);counter++)
	{
		signal(sig_name2number(argv[counter]),sig_handler);

	}

        //wait continuously for any registered signal.
	while(1){
		pause(); 
		if(term_signal_count ==3)
			break;
	}

	fprintf(stderr, "%s:Total signals count = %d\n", argv[0],total_signal_count);  
	return 0;
}




