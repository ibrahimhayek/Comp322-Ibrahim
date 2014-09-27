EXECUTABLES=catcher
SRC=catcher.c
OBJ=catcher.o


CC=gcc
CFLAGS=-g


all: $(EXECUTABLES)


$(EXECUTABLES):
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

clean:
	rm -f $(EXECUTABLES) $(OBJ) *~
