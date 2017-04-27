# Example makefile for CPE464 program 1
#
# 

CC = gcc
CFLAGS = -g -Wall 
#CFLAGS = -g

all: vig ftable

vig: vig.c
	$(CC) $(CFLAGS) -o vig vig.c  

ftable: ftable.c
	$(CC) $(CFLAGS) -o ftable ftable.c -lm 

clean:
	rm -f trace
