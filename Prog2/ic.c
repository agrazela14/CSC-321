#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>
#include "ic.h"

#define BUFFERSIZE 1024 
#define ALPHABETLEN 26 

int main(int argc, char **argv) {
    arguments args;
    args.nVal = 0;
    args.lVals = malloc(10 * sizeof(int));
    args.lCount = 0;
    args.lMax = 10;
    checkArgs(argc, argv, &args);

    output(&args);
    return 0;
}

void checkArgs(int argc, char **argv, arguments *args) {
    int cnt = 1;
    
    if (argc < 2) {
        fprintf(stderr, "Usage: ic N [message length] [key length[key length]..]\n");
        exit(-1);
    }

    args->nVal = atoi(argv[cnt++]);

    while (cnt < argc) {
        args->lVals[args->lCount++] = atoi(argv[cnt++]);
        if (args->lCount == args->lMax) {
            args->lMax *= 2;
            args->lVals = realloc(args->lVals, args->lMax * sizeof(int)); 
        }
    }
}

double calculateIndex(int msgLen, int keyLen) {
    double inverseD = 1/(double)keyLen;
    double nRatio = ((double)msgLen - (double)keyLen) / ((double)msgLen - 1);
    double dRatio = ((double)keyLen - 1) / (double)keyLen; 
    double nRatio2 = (double)msgLen / ((double)msgLen - 1);

    return dRatio * nRatio2 * 0.038 + inverseD * nRatio * 0.066;
}


void output(arguments *args) {
    int ndx;
    double incident;
    printf("Key  Expected IC (N = %d)\n", args->nVal);
    printf("---  -----------------------\n");
    
    for (ndx = 0; ndx < args->lCount; ndx++) {
        incident = calculateIndex(args->nVal, args->lVals[ndx]); 
        printf("%4d %5.4lf\n", args->lVals[ndx], incident);
    }
}

