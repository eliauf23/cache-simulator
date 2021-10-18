CC = g++
CFLAGS = -g -Wall -Wextra -pedantic -std=c++14

csim: main.o csim_fns.o block.o set.o cache.o
	$(CC) -o csim main.o csim_fns.o

main.o: main.cpp csim_fns.h block.h set.h cache.h
	$(CC) $(CFLAGS) -c main.cpp

csim_fns.o: csim_fns.cpp csim_fns.h
	$(CC) $(CFLAGS) -c csim_fns.cpp

block.o: block.cpp block.h
	$(CC) $(CFLAGS) -c block.cpp

set.o: set.cpp set.h
	$(CC) $(CFLAGS) -c set.cpp

cache.o: cache.cpp cache.h
	$(CC) $(CFLAGS) -c cache.cpp

all : csim

clean :
	rm -f csim *.o