#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <linux/limits.h>

/*
 * our children
 */
static pid_t child1 = 0;
static pid_t child2 = 0;

/*
 * function declaration
 */
static int daemonize(void);
static pid_t create_mole(void);
static int kill_mole(pid_t pid);
static int get_rand_num(void);
static void sig_term_handler(int sig);
static void sig_usr1_handler(int sig);
static void sig_usr2_handler(int sig);
/* ---------------------------------------------------------------------------- */

/*
 * Main Entry Point
 */
int main(int argc, char *argv[]) {
    /*
     * become daemon now
     * not checking error, it's ok to run with foreground mode
     */
    daemonize();

    /*
     * install signal handler
     * The daemon handles: SIG_TERM, SIG_USR1, and SIG_USR2
     */
    signal(SIGTERM, sig_term_handler);
    signal(SIGUSR1, sig_usr1_handler);
    signal(SIGUSR2, sig_usr2_handler);

    while (1) {
        sleep(600);
    }

    return 0;
}

/*
 * Upon SIG_TERM, the program kills all child processes and
 * shutdowns the daemon gracefully.
 */
static void sig_term_handler(int sig) {
    int status;

    if (child1 > 0) {
        kill_mole(child1);
        child1 = 0;
    }

    if (child2 > 0) {
        kill_mole(child2);
        child2 = 0;
    }

    exit(0);
}

/*
 * Upon SIG_USR1, the program will kill child process #1
 * randomly create either mole1 or mole 2 if it does not already exists
 *
 */
static void sig_usr1_handler(int sig) {
    /*
     * kill child
     */
    if (child1 > 0) {
        kill_mole(child1);
    }

    /* create random mole */
    child1 = create_mole();
    return;
}

/*
 * Upon SIG_USER2, the program will kill child process #2
 * randomly create either mole1 or mole 2
 */
void sig_usr2_handler(int sig) {
    /*
     * kill child 2
     */
    if (child2 > 0) {
        kill_mole(child2);
    }

    /* create random mole */
    child2 = create_mole();
    return;
}

/*
 * daemonize()
 * become daemon
 */
static int daemonize(void) {
    int i, fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rl;
    struct sigaction sa;

    /*
     * Clear file creation mask.
     */
    umask(0);

    /*
     * Get maximum number of file descriptors.
     */
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
        return -1;

    /*
     * Become a session leader to lose controlling TTY.
     */
    if ((pid = fork()) < 0)
        return -2;
    else if (pid != 0) /* parent */
        exit(0);

    /*
     * Create a new session
     */
    setsid();

    /*
     * Ensure future opens won't allocate controlling TTYs.
     */
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        return -3;
    }

    if ((pid = fork()) < 0) {
        return -4;
    } else if (pid != 0) { /* parent */
        exit(0);
    }

    /*
     * Change the current working directory to the root so
     * we won't prevent file systems from being unmounted.
     */
    if (chdir("/") < 0) {
        return -5;
    }

    /*
     * last chance to print some information
     */
    fprintf(stdout, "pid: %d\n", getpid());
    /*
     * Close all open file descriptors.
     */
    if (rl.rlim_max == RLIM_INFINITY) {
        rl.rlim_max = 1024;
    }

    for (i = 0; i < rl.rlim_max; i++) {
        close(i);
    }

    /*
     * Attach file descriptors 0, 1, and 2 to /dev/null.
     */
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    return 0;
}

/*
 * create_mole()
 * create mole process located at ${HOME}
 */
static pid_t create_mole(void) {
    pid_t pid = 0;
    char *arglist[2];
    char mole_path[PATH_MAX];

    snprintf(mole_path, sizeof(mole_path), "%s/mole", getenv("HOME"));

    /*
     * check if mole is exist and exec-able
     */
    if (access(mole_path, X_OK) != 0) {
        return pid;
    }

    /*
     * create mole process randomly
     */
    if (get_rand_num() % 2) {
        arglist[0] = "mole1";
    } else {
        arglist[0] = "mole2";
    }
    arglist[1] = NULL;

    if ((pid = fork()) < 0) {
        return pid;
    }

    if (pid == 0) { /* child process */
        if (execv(mole_path, arglist) < 0) {
            exit(1);
        }
    }

    return pid;
}

/*
 * get_rand_num()
 * Get a random number so that we can create mole randomly base on that number
 */
static int get_rand_num(void) {
    time_t seed = time(NULL);
    srand(seed);
    return rand();
}

/*
 * kill_mole()
 * send SIGTERM signal to child process and request status information from a child
 */
static int kill_mole(pid_t pid) {
    int status;

    if (kill(pid, SIGTERM) == 0) {
        return waitpid(pid, &status, 0);
    }
    return -1;
}
