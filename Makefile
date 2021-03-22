CC = gcc
CFLAGS = -Wall

allocate: allocate.o pq.o
	$(CC) -o allocate allocate.o pq.o -g

allocate.o: allocate.c allocate.h pq.h
	$(CC) -c allocate.c -g

pq.o: pq.c pq.h
	$(CC) -c pq.c -g
	
clean :
	rm -f *.o $(objects) allocate 
	