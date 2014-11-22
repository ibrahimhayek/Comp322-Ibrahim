#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int randnum(void);
/*
 * A program that creates a file of integers that represents a NxN matrix
 * The matrix will be stored in file as row-major order
 * Usage:  matrix_gen size >matrix_file.size
 * Generates a matrix of size "size x size" that contains a random set of integers
 * from -100 .. 100
 */
int main(int argc, char *argv[])
{
    if(argc != 2) 
    {
        fprintf(stderr, "Usage: %s <MATRIX_SIZE>\n", argv[0]);
        return -1;
    }

    int size = atoi(argv[1]);
    int i, j;
    int n;
    if(size <= 0) {
        fprintf(stderr, "Invalid matrix size %d\n", size);
        return -1;
    }
    
    /*
     * populate matrix with data and print out in row-major order
     */
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            n = randnum();
            fwrite(&n, sizeof(int), 1, stdout);
        }
    }

    return 0;
}

/*
 * randnum() : return a random number between -100 .. 100
 */
int randnum(void)
{
    static int i=0;
    srand(time(NULL)+(++i));
    return (rand()%201) - 100;
}
