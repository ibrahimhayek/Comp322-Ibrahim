all: version1

version1: zero-one.c
	gcc -o version1 zero-one.c -lm


