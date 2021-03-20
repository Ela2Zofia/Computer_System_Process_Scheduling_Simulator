CC = gcc
CFLAGS = -Wall

allocate: allocate.o pq.o
	$(CC) -o allocate allocate.o pq.o -g

allocate.o: allocate.c allocate.h
	$(CC) -c allocate.c

pq.o: pq.c pq.h
	$(CC) -c pq.c
	
clean :
	rm *.o $(objects) allocate 
	