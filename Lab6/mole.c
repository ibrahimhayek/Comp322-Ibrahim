#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/limits.h>

/*
 * a file pointer (lab6.log)
 */
static FILE *fp = NULL;

/*
 * a cleanup function before the mole exits
 */
void clean_resource(void) {
    if (fp) {
        fclose(fp); /* close file */
        fp = NULL;
    }
}

/*
 * Main Entry Point for Mole program
 * This program simply writes a signal line to a well-known log file (~/lab6.log).
 * The program writes either the string: Pop mole1 | Pop mole2
 * The mole program knows what to write based upon the value of argv[0]
 */
int main(int argc, char *argv[]) {
    char log_path[PATH_MAX];
    /*
     * Open log file for appending
     */
    snprintf(log_path, sizeof(log_path), "%s/lab6.log", getenv("HOME"));

    fp = fopen(log_path, "a");
    if (fp == NULL) {
        fprintf(stderr, "fopen(%s) failed (%s)\n", log_path, strerror(errno));
        return -1;
    }

    atexit(clean_resource);

    fprintf(fp, "Pop %s\n", argv[0]);
    fflush(fp);

    while (1) {
        sleep(5);
    }

    return 0;
}
