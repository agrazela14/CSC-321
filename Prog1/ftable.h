#ifndef FTABLE_H
#define FTABLE_H

typedef struct {
    int verbose;
    int skip;
    int period;
    int inFile;
    int outFile;
} arguments; 

int main(int argc, char **argv);

void table(arguments *args);

void printout(int *alphabetTable, int totalRead, arguments *args);

void checkArgs(int argc, char **argv, arguments *args);

#endif
