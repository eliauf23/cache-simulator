CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=gnu11

csim: main.o csim_fns.o
	$(CC) -o csim main.o csim_fns.o

main.o: main.c csim_fns.h
	$(CC) $(CFLAGS) -c main.c

csim_fns.o: csim_fns.c csim_fns.h
	$(CC) $(CFLAGS) -c csim_fns.c

all : csim

clean :
	rm -f csim *.o