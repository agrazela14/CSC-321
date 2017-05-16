# Example makefile for CPE464 program 1
#
# 

CC = gcc
CFLAGS = -g -Wall 
#CFLAGS = -g

all: vig ftable ic kasiski

ic: ic.c
	$(CC) $(CFLAGS) -o ic ic.c -lm 

ftable: ftable.c
	$(CC) $(CFLAGS) -o ftable ftable.c -lm 

kasiski: kasiski.c
	$(CC) $(CFLAGS) -o kasiski kasiski.c -lm 

clean:
	rm -f trace
