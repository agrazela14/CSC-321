#ifndef FTABLE_H
#define FTABLE_H

typedef struct {
    int verbose;
    int distanceMode;
    int minSubstr;
    FILE *inFile;
    FILE *outFile;
} arguments; 

int main(int argc, char **argv);

void checkArgs(int argc, char **argv, arguments *args);

#endif
