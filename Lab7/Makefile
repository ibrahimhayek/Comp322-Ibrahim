

all: matrix_gen matrix_add

matrix_gen: matrix_gen.c
	gcc -g -Wall -Werror -o matrix_gen matrix_gen.c


matrix_add: matrix_add.c
	gcc -g -Wall -Werror -o matrix_add matrix_add.c -lrt

clean:
	rm -f matrix_gen.o matrix_add.o matrix_gen matrix_add


