CC = gcc
CFLAGS = -Wall

allocate: allocate.c allocate.h
	$(CC) -o allocate allocate.c -g