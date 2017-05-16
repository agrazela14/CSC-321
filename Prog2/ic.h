#ifndef IC_H
#define IC_H

typedef struct {
    int nVal;
    int *lVals;
    int lCount;
    int lMax;
} arguments; 

int main(int argc, char **argv);

void checkArgs(int argc, char **argv, arguments *args);

double calculateIndex(int msgLen, int keyLen);

void output(arguments *args);

#endif
