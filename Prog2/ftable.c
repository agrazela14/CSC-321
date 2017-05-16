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
    args.inFile = stdin;
    args.outFile = stdout;
    checkArgs(argc, argv, &args);

    table(&args);

    return 0;
}

void table(arguments *args) {
    char curChar = 0; 
    int *alphabetCount = malloc(ALPHABETLEN * sizeof(int));
    int skipNdx = 0, periodNdx = args->period, ndx = 0;
    int totalRead = 0;

    for (ndx = 0; ndx < ALPHABETLEN; ndx++) {
        alphabetCount[ndx] = 0;
    }
    
    while (curChar != EOF && skipNdx < args->skip) { 
        curChar = fgetc(args->inFile);
        while (!isalpha(curChar)) {
            curChar = fgetc(args->inFile);
            if (curChar == EOF) {
                break;
            }
        } 
        skipNdx++; 
    }
    while (curChar != EOF) {
        curChar = fgetc(args->inFile);
        while (!isalpha(curChar)) {
            curChar = fgetc(args->inFile);
            if (curChar == EOF) {
                break;
            }
        } 
        if (periodNdx < args->period - 1) {
            periodNdx++;
        }
        else if (isalpha(curChar)) {
            curChar = toupper(curChar);
            alphabetCount[curChar - 'A']++;
            totalRead += 1;
            periodNdx = 0;
        }
    }
    printout(alphabetCount, totalRead, args);
    free(alphabetCount);
}

double calculateIndex(int *alphabetCount, int totalRead) {
    double mult = 1 / ((double)totalRead * ((double)totalRead - 1));
    int ndx;
    int sum = 0;

    for (ndx = 0; ndx < ALPHABETLEN; ndx++) {
        sum += alphabetCount[ndx] * (alphabetCount[ndx] - 1);
    }
    return (double)sum * mult; 
}

void printout(int *alphabetTable, int totalRead, arguments *args) {
    char buf[BUFFERSIZE];
    char asterisk[101];
    double ic;// = calculateIndex(alphabetTable, totalRead);
    int i;

    for (i = 0; i < 101; i++) {
        asterisk[i] = '*';
    } 
    asterisk[100] = '\0';

    sprintf(buf, "Total chars: %d\n", totalRead);
    fputs(buf, args->outFile);
    if (totalRead == 0) {
        totalRead = 1;
    }
    for (i = 0; i < ALPHABETLEN; i++) {
        sprintf(buf, "%C:% 9d (%5.2lf%%) %.*s\n", 'A' + i, alphabetTable[i], 
         100 * ((double)alphabetTable[i] / totalRead), 
          (int)ceil((double)alphabetTable[i] / totalRead * 100), asterisk);
        fputs(buf, args->outFile);
    }
    //fflush(args->outFile);
    ic = calculateIndex(alphabetTable, totalRead);
    fprintf(args->outFile, "Index of Conicidence: %5.4lf\n", ic);
}

void checkArgs(int argc, char **argv, arguments *args) {
    int cnt = 1;

    while (cnt < argc) {
        if (strcmp((argv)[cnt], "-v") == 0) {
            args->verbose = 1;
        }

        else if (strcmp((argv)[cnt], "-s") == 0) {
            args->skip = atoi(argv[++cnt]);
        }

        else if (strcmp((argv)[cnt], "-p") == 0) {
            args->period = atoi(argv[++cnt]);
        }
        
        else if (args->inFile == stdin) {
            args->inFile = fopen(argv[cnt], "r"); 
            if (args->inFile < 0) {
                fprintf(stderr, "Bad input file\n");
                exit (-1);
            }
        }
        else {
            args->outFile = fopen(argv[cnt], "w+"); 
        }
        cnt++;
    }
    if (args->verbose) {
        printf("Verbosity Engaged\n");
        if (args->skip) {
            printf("Skip mode, skipping first %d\n", args->skip); 
        }
        if (args->period > 1) {
            printf("Period mode, every %d characters\n", args->period); 
        }
    }
}

