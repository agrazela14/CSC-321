#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>
#include "ftable.h"

#define BUFFERSIZE 1024 
#define ALPHABETLEN 26 

int main(int argc, char **argv) {
    arguments args;
    args.verbose = 0;
    args.skip = 0;
    args.period = 1;
    args.inFile = STDIN_FILENO;
    args.outFile = STDOUT_FILENO;
    checkArgs(argc, argv, &args);

    table(&args);

    return 0;
}

void table(arguments *args) {
    char curChar[args->period]; 
    int alphabetCount[ALPHABETLEN] = {};
    int readBytes = 0;// = fread(plainText, 1, BUFFSIZE, args->inFile); 
    int totalRead = 0;

    if (args->skip > 0) {
        char dump[args->skip];
        read(args->inFile, dump, args->skip);
    }
    
    do {
        readBytes = read(args->inFile, curChar, args->period);
        curChar[0] = toupper(curChar[0]);
        if (curChar[0] <= 'Z' && curChar[0] >= 'A') {
            alphabetCount[curChar[0] - 'A']++;
            totalRead += 1;
        }
    } while (readBytes == args->period);
    printout(alphabetCount, totalRead, args);
}

void printout(int *alphabetTable, int totalRead, arguments *args) {
    char buf[BUFFERSIZE];
    char asterisk[101];
    int i;

    for (i = 0; i < 101; i++) {
        asterisk[i] = '*';
    } 
    asterisk[100] = '\0';

    sprintf(buf, "Total chars: %d\n", totalRead);
    write(args->outFile, buf, strlen(buf));
    for (i = 0; i < ALPHABETLEN; i++) {
        sprintf(buf, "%C:% 9d (%5.2lf%%) %.*s\n", 'A' + i, alphabetTable[i], 
         100 * ((double)alphabetTable[i] / totalRead), (int)ceil((double)alphabetTable[i] / totalRead * 100), asterisk);
        /*
        for (int j = 0; j < alphabetTable[i]; j++) {
            sprintf(buf + strlen(buf) - 1, "*");
        }
        sprintf(buf + strlen(buf), "\n");
        */
        write(args->outFile, buf, strlen(buf));
    }
}

void checkArgs(int argc, char **argv, arguments *args) {
    int cnt = 1;

    while (cnt < argc) {
        if (strcmp((argv)[cnt], "-v") == 0) {
            args->verbose = 1;
            printf("Verbosity Engaged\n");
        }

        else if (strcmp((argv)[cnt], "-s") == 0) {
            args->skip = atoi(argv[++cnt]);
            if (args->verbose) {
                printf("Skip mode, skipping first %d\n", args->skip); 
            }
        }

        else if (strcmp((argv)[cnt], "-p") == 0) {
            args->period = atoi(argv[++cnt]);
            if (args->verbose) {
                printf("Period mode, counting every %d characters\n", args->period); 
            }
        }
        
        else if (args->inFile == STDIN_FILENO) {
            args->inFile = open(argv[cnt], O_RDONLY); 
        }
        else {
            args->outFile = open(argv[cnt], O_RDWR | O_TRUNC | O_CREAT, S_IRWXU); 
        }
        cnt++;
    }
}

