#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>
#include "kasiski.h"

int main(int argc, char **argv) {
    arguments args;
    args.verbose = 0;
    args.distanceMode = 0;
    args.minSubStr = 3;
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
    int ndx, ndx2;
    int used = 0, dataMax = 10;
    subStringData *data;// = malloc(10 * sizeof(subStringData));
    //subStringData *dataIndex;// = data;

    fseek(args->inFile, 0L, SEEK_END);
    fileLen = ftell(args->inFile);
    rewind(args->inFile);

    text = malloc(fileLen + 1);
    data = malloc(10 * sizeof(subStringData));
    //dataIndex = data;
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
    if (args->verbose) {
        printf("Filelen: %d Text:\n%s\n", fileLen, text);
    }

    //This tells us how many min length subStr there are, as well as putting them in an array
    //Now keep going over it until there aren't any more added
    while (newSubStr != 0) {
        newSubStr = initialPass(args, &data/*Index*/, text, fileLen, &used, &dataMax);
        subStrUsed += newSubStr;
        used = newSubStr;
        args->minSubStr += 1;
        //dataIndex += newSubStr * sizeof(data);
    } 
    args->minSubStr = originalMinLen; 
    printout(args, data, subStrUsed); 
    free(data);    
    free(text); 
}

void printout(arguments *args, subStringData *data, int subStrs) {
    int ndx;

    if (args->distanceMode) {
        fprintf(args->outFile, "Length   Count   Word   Distance\n");  
    }
    else {
        fprintf(args->outFile, "Length   Count   Word   Locations\n");  
    }
    fprintf(args->outFile, "======   =====   ====   ==========\n");  
    
    qsort(data, subStrs, sizeof(subStringData), compare_function); 
    
    //For each one print length, count, the substr, and then either locations or distances
    for (ndx = 0; ndx < subStrs; ndx++) {
        variablePrint(args, &data[ndx]); 
    }
}

void variablePrint(arguments *args, subStringData *data) {
    int *values;
    int numDist = 0, ndx, ndx2, count = 0;

    fprintf(args->outFile, "%5d   %4d   %s   ", 
     data->strlen, data->count, data->substr); 

    if (args->distanceMode) {
        //print out the distances between every location in data
        for (ndx = data->count - 1; ndx > 0; ndx--) {
            numDist += ndx;
        }
        values = malloc(sizeof(int) * numDist);
        for (ndx = 0; ndx < data->count; ndx++) {
            for (ndx2 = ndx; ndx2 < data->count; ndx2++) {
                values[count++] = data->locations[ndx2] - data->locations[ndx];
            }
        }
        for (ndx = 0; ndx < numDist; ndx++) {
            if (values[ndx] >= data->strlen) {
                fprintf(args->outFile, "%d ", values[ndx]);
            }
        }
        fprintf(args->outFile, "\n");   
    }
    else {
        for (ndx = 0; ndx < data->count; ndx++) {
            fprintf(args->outFile, "%d ", data->locations[ndx]);
        }
        fprintf(args->outFile, "\n");
    }
}

//A before B return negative
int compare_function(const void *a, const void *b) {

    subStringData *dataA = (subStringData *)a;
    subStringData *dataB = (subStringData *)b;

    if (dataA->strlen != dataB->strlen) {
        return dataB->strlen - dataA->strlen;
    }

    if (dataA->count != dataB->count) {
        return dataB->count - dataA->count;
    }
    return strcmp(dataB->substr, dataA->substr);

}

int initialPass(arguments *args, subStringData **data, char *text, 
 int fileLen, int *used, int *dataMax) {
    int ndx, ndx2;
    //int used = 0, dataMax = 10;
    int found = 0;
    int start = *used;
    int multipleInstances = 0;
    char subStr[args->minSubStr + 1];
    subStringData *temp;
    //Find all min length substrings 
    for (ndx = 0; ndx < fileLen - args->minSubStr; ndx++) {
        memcpy(subStr, text + ndx, args->minSubStr);
        subStr[args->minSubStr] = '\0';
        for (ndx2 = 0; ndx2 < *used; ndx2++) {
            if (strcmp((*data)[ndx2].substr, subStr) == 0) {
                if (args->verbose) {
                    printf("Found in data[%d]: %s, count of %d\n", ndx2, subStr, (*data)[ndx2].count + 1);
                }
                if ((*data)[ndx2].count == 1) {
                    multipleInstances++;
                }
                (*data)[ndx2].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            (*data)[*used].locations[(*data)[*used].count] = ndx;
            (*data)[*used].count = 1;
            (*data)[*used].strlen = args->minSubStr;
            if (args->verbose) {
                printf("Copying into data[%d]: %s\n", *used, subStr);
            }
            strcpy((*data)[*used].substr, subStr); 
            (*used)++;
            if (*used == *dataMax) {
                if (args->verbose) {
                    printf("resized\n");
                }
                *dataMax *= 2;
                (*data) = realloc(*data, *dataMax * sizeof(subStringData)); 
            }
        }
        found = 0;
    } 
    //Now we should have all the min length subStrings 
    //so we can go ahead and get rid of the
    //Ones with a count of 0
    temp = malloc(multipleInstances * sizeof(subStringData));
    ndx2 = 0;
    for (ndx = start; ndx < *used; ndx++) {
        if ((*data)[ndx].count > 1) {
            memcpy(&(temp[ndx2]), &(*data)[ndx], sizeof(subStringData));
            ndx2++;
        }
    }
    free(temp);
    if (args->verbose) {
        printf("temp freed\n");
    }
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
            args->minSubStr = atoi(argv[++cnt]);
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
}

