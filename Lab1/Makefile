# Name: Ibrahim Hayek
# Project: Lab 1: Time 4 Baby and Me

# EXECUTABLES is a Make Variable (similar to an environment variable) that lists all the
# "executables" associated with the lab. 
EXECUTABLES=time-4-baby-and-me
SRC=time-4-baby-and-me.c
OBJ=time-4-baby-and-me.o


# The CC and CFLAGS variables are defined to ensure I'm using the native compiler and
# that the correct command-line options are set
CC=gcc
CFLAGS=-g


# Here is my default target for the Makefile.  When I type make, this first target will
# be created. I.e., all the executables
all: $(EXECUTABLES)


# each time my source file is modified.
$(EXECUTABLES):
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

clean:
	rm -f $(EXECUTABLES) $(OBJ) *~
