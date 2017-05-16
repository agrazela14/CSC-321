#ifndef FTABLE_H
#define FTABLE_H

typedef struct {
    int verbose;
    int skip;
    int period;
    FILE *inFile;
    FILE *outFile;
} arguments; 

int main(int argc, char **argv);

void table(arguments *args);

void printout(int *alphabetTable, int totalRead, arguments *args);

double calculateIndex(int *alphabetCount, int totalRead);

void checkArgs(int argc, char **argv, arguments *args);

#endif
