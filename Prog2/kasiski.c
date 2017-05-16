#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>
#include "kasiski.h"

#define SUBSTRLEN 255
#define LOCATIONMAX 255

typedef struct{
    char substr[SUBSTRLEN];
    int locations[LOCATIONMAX];
    int strlen;
    int count;
} subStringData;

int main(int argc, char **argv) {
    arguments args;
    args.verbose = 0;
    args.distanceMode = 0;
    args.minSubstr = 3;
    args.inFile = stdin;
    args.outFile = stdout; 
    checkArgs(argc, argv, &args);
    kasiski(&args);

    return 0;
}

//Sweep through taking every single min Len substr
//Then discard all with no repeats
//Then look at each of those locations for 1 longer substrs
//Repeat until there are no substrings found with more than 1 count


//Actually maybe all this is overdoing it, maybe I just need to have it operate
//Like the first pass until it doesn't find any
void kasiski(arguments *args) {
    int fileLen;
    int originalMinLen = args->minSubStr;
    char *text, curChar;
    int subStrUsed = 0, newSubStr = 1;
    int fileNdx, subStrNdx;
    int subStrLen = args->minSubstr + 1;
    int used = 0, dataMax = 10;
    subStringData *data = malloc(10 * sizeof(subStringData));
    subStringData *dataIndex = data;

    fseek(args->inFile, 0L, SEEK_END);
    fileLen = ftell(args->inFile);
    rewind(args->inFile);
    text = malloc(fileLen);
    ndx2 = 0;
     
    for (ndx = 0; ndx < fileLen; ndx++) {
        curChar = fgetc(args->inFile);
        if (isalpha(curChar)) {
            text[ndx2] = toupper(curChar);
            ndx2++;
        }
    } 
    fileLen = ndx2;
    text[ndx2] = '\0';
    //fread(text, fileLen, 1, args->inFile);

    /*
    subStrUsed = initialPass(args, &data, text, fileLen, *used, *dataMax);
    newSubStr = subStrUsed;
    */
    //This tells us how many min length subStr there are, as well as putting them in an array
    //Now keep going over it until there aren't any more added
    while (newSubStr != 0) {
        newSubStr = initialPass(args, &dataIndex, text, fileLen, *used, *dataMax);
        subStrUsed += newSubStr;
        args->minSubStr += 1;
        dataIndex += newSubStr * sizeof(data);
        /*
        mewSubStr = continuedPass(args, &data, text, 
         fileLen, subStrUsed - newSubStr, subStrUsed, subStrLen); 
        subStrLen++;
        */
    } 
    args->minSubStr = originalMinLen; 
    printout(args, data, subStrUsed); 
    free(data);    
    free(text); 

}

//Ordering these might be a pain...
void printout(arguments *args, subStringData *data, int subStrs) {
    int ndx;

    if (args->distanceMode) {
        fprintf(args->outFile, "Length   Count   Word   Locations\n");  
    }
    else {
        fprintf(args->outFile, "Length   Count   Word   Distance\n");  
    }
    fprintf(args->outFile, "======   =====   ====   ==========\n");  
    
    qsort(data, subStrs, sizeof(subStringData), compare_function); 
    
    //For each one print length, count, the substr, and then either locations or distances
    for (ndx = 0; ndx < subStrs; ndx++) {
        fprintf(args->outFile, "%5d   %4d   %s   ", 
         data[ndx]->strLen, data[ndx]->count, data[ndx]->subStr); 
    }
}

//A before B return negative
int compare_function(const void *a, const void *b) {

    subStringData *dataA = (subStringData *)a;
    subStringData *dataB = (subStringData *)B;

    if (dataA->strLen != dataB->strLen) {
        return dataB->strLen - dataA->strLen;
    }

    if (dataA->count != dataB->count) {
        return dataB-count - dataA->count;
    }
    return strcmp(dataB->subStr, dataA->subStr);

}

//Checking from the dataStart to dataEnd indicies, what are the substrings that follow up
//dataStart is where the previous length subStrs start
/*
int continuedPass(arguments *args, subStringData **data, 
 char *text, int fileLen, int dataStart, int dataEnd, int length) {
    int ndx;
    
    for (ndx = dataStart; ndx < dataEnd; ndx++) {
         
    
} 
*/

int initialPass(arguments *args, subStringData **data, char *text, 
 int fileLen, int *used, int *dataMax) {
    int ndx, ndx2;
    int used = 0, dataMax = 10;
    int found = 0;
    int multipleInstances = 0;
    char subStr[args->minSubstr + 1];
    subStringData *temp;
    //Find all min length substrings 
    for (ndx = 0; ndx < fileLen - args->minSubstr; ndx++) {
        memcpy(subStr, text + ndx, args->minSubstr);
        subStr[args->minSubstr] = '\0';
        for (ndx2 = 0; ndx2 < *used; ndx2++) {
            if (strcmp(data[ndx2]->substr, subStr) == 0) {
                if ((*data)[ndx]->count == 1) {
                    multipleInstances++;
                }
                data[ndx2]->count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            (*data)[*used]->locations[(*data)[*used]->count] = ndx;
            (*data)[*used]->count = 1;
            strcpy(data[*used]->substr, subStr); 
            (*used)++;
            if (*used == *dataMax) {
                *dataMax *= 2;
                (*data) = realloc(data, *dataMax * sizeof(subStringData)); 
            }
        }
    } 
    //Now we should have all the min length subStrings 
    //so we can go ahead and get rid of the
    //Ones with a count of 0
    temp = malloc(multipleInstances * sizeof(subStringData));
    ndx2 = 0;
    for (ndx = 0; ndx < *used; ndx++) {
        if ((*data)[ndx]->count > 1) {
            memcpy(temp[ndx2], (*data)[ndx], sizeof(subStringData));
            ndx2++;
        }
    }
    
    memcpy(*data, temp, multipleInstances * sizeof(subStringData)); 
    free(temp);
    /*
    free(*data);
    *data = temp;
    */
    return multipleInstances;
} 


void checkArgs(int argc, char **argv, arguments *args) {
    int cnt = 1;

    while (cnt < argc) {
        if (strcmp((argv)[cnt], "-v") == 0) {
            args->verbose = 1;
        }

        else if (strcmp((argv)[cnt], "-d") == 0) {
            args->distanceMode = 1;
        }

        else if (strcmp((argv)[cnt], "-m") == 0) {
            args->minSubstr = atoi(argv[++cnt]);
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

