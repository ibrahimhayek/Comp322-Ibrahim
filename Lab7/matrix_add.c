#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <aio.h>
#include <time.h>

/*
 * Function prototypes
 */
int matrix_add(char *block, int block_size, int scalar);
int randnum(void);

/*
 * Main Entry Point
 */
int main(int argc, char *argv[])
{
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <SIZE> <BLOCKS>\n", argv[0]);
        return -1;
    }

    int size = atoi(argv[1]);
    int blocks = atoi(argv[2]);
    
    if(size <= 0) {
        fprintf(stderr, "invalid size %d\n", size);
        return -1;
    }

    if(blocks <= 0) {
        fprintf(stderr, "invalid blocks %d\n", blocks);
        return -1;
    }
    
    if(size%blocks) {
        fprintf(stderr, "invalid size & blocks values %d/%d\n", size, blocks);
        return -1;
    }
    
    int block_size = size/blocks;
    const int BUFFER_SIZE = block_size*block_size*sizeof(int);
    char *last = malloc(BUFFER_SIZE);
    char *current = malloc(BUFFER_SIZE);
    char *next = malloc(BUFFER_SIZE);

    struct aiocb cb_last;
    struct aiocb cb;
    struct aiocb cb_next;
    
    int i;
    int n;
    int scalar = randnum();
    int blocknum = 0;
    clock_t start_time, end_time;
    float elapsedTime = 0;
    start_time = clock();
    
    fprintf(stderr, "READ REQ %d\n", blocknum);
    memset(&cb, 0x00, sizeof(struct aiocb));
    cb.aio_nbytes = BUFFER_SIZE;
    cb.aio_fildes = fileno(stdin);
    cb.aio_offset = (blocknum)*BUFFER_SIZE;
    cb.aio_buf = current;
    if (aio_read(&cb) == -1) {
        fprintf(stderr, "Unable to create request\n");
    }
    
    while(aio_error(&cb) == EINPROGRESS) {
        /* the request has not been completed yet */
        ;
    }
    fprintf(stderr, "READ RETURN %d\n", blocknum);
    n = aio_return(&cb);
    if(n == -1) {
        fprintf(stderr, "error read block %d\n", blocknum);
    }
    
    for(i = 0; i < blocks*blocks-1; i++) {
            /*
             * enqueue next read request
             */
            fprintf(stderr, "READ REQ %d\n", blocknum + 1);
            memset(&cb_next, 0x00, sizeof(struct aiocb));
            cb_next.aio_nbytes = BUFFER_SIZE;
            cb_next.aio_fildes = fileno(stdin);
            cb_next.aio_offset = (blocknum+1)*BUFFER_SIZE;
            cb_next.aio_buf = next;
            if (aio_read(&cb_next) == -1) {
                fprintf(stderr, "Unable to create request\n");
            }
            
            fprintf(stderr, "CALCULATING %d\n", blocknum);
            matrix_add(current, block_size, scalar);
            
            memcpy(last, current, BUFFER_SIZE);
            blocknum++;
            
            fprintf(stderr, "WRITE REQ %d\n", blocknum-1);
            memset(&cb_last, 0x00, sizeof(struct aiocb));
            cb_last.aio_nbytes = BUFFER_SIZE;
            cb_last.aio_fildes = fileno(stdout);
            cb_last.aio_offset = (blocknum-1)*BUFFER_SIZE;
            cb_last.aio_buf = last;
            if(aio_write(&cb_last) == -1) {
                fprintf(stderr, "write queue request error\n");
            }
            
            while(aio_error(&cb_last) == EINPROGRESS) {
                ;
            }
            fprintf(stderr, "WRITE RETURN %d\n", blocknum-1);
            n = aio_return (&cb_last);
            if(n == -1) {
                fprintf(stderr, "error write block %d\n", blocknum-1);
            }
            
            while(aio_error(&cb_next) == EINPROGRESS) {
                ;
            }
            fprintf(stderr, "READ RETURN %d\n", blocknum);
            n = aio_return (&cb_next);
            if(n == -1) {
                fprintf(stderr, "error read block %d\n", blocknum);
            }
            memcpy(current, next, BUFFER_SIZE); /* for future calculating */
    }
    
    fprintf(stderr, "CALCULATING %d\n", blocknum);
    matrix_add(current, block_size, scalar);
    
    fprintf(stderr, "WRITE REQ %d\n", blocknum);
    memset(&cb, 0x00, sizeof(struct aiocb));
    cb.aio_nbytes = BUFFER_SIZE;
    cb.aio_fildes = fileno(stdout);
    cb.aio_offset = (blocknum)*BUFFER_SIZE;
    cb.aio_buf = current;
    if(aio_write(&cb) == -1) {
        fprintf(stderr, "write queue request error\n");
    }
    
    while(aio_error(&cb) == EINPROGRESS) {
        ;
    }
    fprintf(stderr, "WRITE RETURN %d\n", blocknum);
    n = aio_return (&cb);
    if(n == -1) {
        fprintf(stderr, "error write block %d\n", blocknum);
    }
    
    end_time = clock();
    elapsedTime = (float)(end_time - start_time) / (float)CLOCKS_PER_SEC * 1000.0f;
    fprintf(stderr, "%.03f miliseconds\n", elapsedTime);
    
    free(last);
    free(current);
    free(next);
    
    return 0;
}

/*
 *  matrix_add(): add scalar to a matrix in row-major order
 */
int matrix_add(char *block, int block_size, int scalar) {
    int i = 0;
    for(i = 0; i < block_size*block_size; i++) {
        *(((int *)block)+i) += scalar;
    }
    return 0;
}

/*
 * randnum() : generate a random number range -100 .. 100
 */
int randnum(void)
{
    static int i=0;
    srand(time(NULL)+(++i));
    return (rand()%201) - 100;
}
