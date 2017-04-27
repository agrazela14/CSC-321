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
    args.inFile = stdin;//STDIN_FILENO;
    args.outFile = stdout;STDOUT_FILENO;
    checkArgs(argc, argv, &args);

    table(&args);

    return 0;
}

void table(arguments *args) {
    char curChar = 0;//[args->period]; 
    int alphabetCount[ALPHABETLEN] = {};
    int skipNdx = 0, periodNdx = args->period;
    //int readBytes = 0;// = fread(plainText, 1, BUFFSIZE, args->inFile); 
    int totalRead = 0;
    
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
        else {
            alphabetCount[curChar - 'A']++;
            totalRead += 1;
            periodNdx = 0;
        }
    }


    /*
    if (args->skip > 0) {
        for(ndx = 0; ndx > args->skip; ndx++) {
            fgetc(args->inFile);
        }
    }
    
    while ((curChar = fgetc(args->inFile)) != EOF) {
        curChar = toupper(curChar);
        if (curChar <= 'Z' && curChar >= 'A') {
            alphabetCount[curChar - 'A']++;
            totalRead += 1;
        }
        for (ndx = 0; ndx < args->period - 1; ndx++) {
            fgetc(args->inFile);
        }
    }
    */
    
    /*     
    do {
        readBytes = read(args->inFile, curChar, args->period);
        curChar[0] = toupper(curChar[0]);
        if (curChar[0] <= 'Z' && curChar[0] >= 'A') {
            alphabetCount[curChar[0] - 'A']++;
            totalRead += 1;
        }
    } while (readBytes == args->period);
    */
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
    fputs(buf, args->outFile);
    //write(args->outFile, buf, strlen(buf));
    for (i = 0; i < ALPHABETLEN; i++) {
        sprintf(buf, "%C:% 9d (%5.2lf%%) %.*s\n", 'A' + i, alphabetTable[i], 
         100 * ((double)alphabetTable[i] / totalRead), 
          (int)ceil((double)alphabetTable[i] / totalRead * 100), asterisk);
        /*
        for (int j = 0; j < alphabetTable[i]; j++) {
            sprintf(buf + strlen(buf) - 1, "*");
        }
        sprintf(buf + strlen(buf), "\n");
        */
        fputs(buf, args->outFile);
        //write(args->outFile, buf, strlen(buf));
    }
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

