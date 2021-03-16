CC = gcc
CFLAGS = -Wall

allocate: allocate.c
	$(CC) -o allocate allocate.c