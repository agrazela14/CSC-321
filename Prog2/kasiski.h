#ifndef KASISKI_H
#define KASISKI_H

#define SUBSTRLEN 16 
#define LOCATIONMAX 16 

typedef struct {
    char substr[SUBSTRLEN];
    int locations[LOCATIONMAX];
    int strlen;
    int count;
} subStringData;

typedef struct {
    int verbose;
    int distanceMode;
    int minSubStr;
    FILE *inFile;
    FILE *outFile;
} arguments;

int main(int argc, char **argv);


void kasiski(arguments *args);

void printout(arguments *args, subStringData *data, int subStrs);

void variablePrint(arguments *args, subStringData *data);

int compare_function(const void *a, const void *b);

int initialPass(arguments *args, subStringData **data, char *text, 
 int fileLen, int *used, int *dataMax);
 
void checkArgs(int argc, char **argv, arguments *args);
 

#endif
