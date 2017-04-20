#ifndef VIG_H
#define VIG_H

typedef struct {
    int verbose;
    int decipher;
    char *key;
    int inFile;
    int outFile;
} arguments; 

int main(int argc, char **argv);

void encipher(arguments *args);

//void decipher(arguments *args);

void checkArgs(int argc, char **argv, arguments *args);

#endif
