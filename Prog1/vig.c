#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vig.h"

#define BUFFERSIZE 1024 
#define ALPHABETLEN 26 

/*
typedef struct {
    int verbose;
    int decipher;
    char *key;
    FILE *inFile;
    FILE *outFile;
} arguments; 
*/

int main(int argc, char **argv) {
    arguments args;
    args.verbose = 0;
    args.decipher = 0;
    args.key = calloc(1, BUFFERSIZE); //I don't actually know how big the key is allowed to be
    args.inFile = fopen(stdin, "r");
    args.outFile = fopen(stdout, "w+");
    checkArgs(argc, argv, &args);

    if (args.decipher == 0) {
        encipher(&args);
    }
    else {
        decipher(&args);
    }

    free(args.key);
    return 0;
}

void encipher(arguments *args) {
    char plainText[BUFFERSIZE]; 
    char cipherText[BUFFERSIZE]; 
    int readBytes;// = fread(plainText, 1, BUFFSIZE, args->inFile); 
    int keyLen = strlen(args->key);
    int keyCur = 0;
    
    do {
        readBytes = fread(plainText, 1, BUFFERSIZE, args->inFile); 
        for (ndx = 0; ndx < readBytes; ndx++) {
            if ('a' <= plainText[ndx] && 'Z' >= plainText[ndx]) {
                cipherText[ndx] = toUpper(plainText[ndx]); 
                cipherText[ndx] += args->key[keyCur++];
                if (cipherText[ndx] > 'Z') {
                    cipherText[ndx] -= ALPHABETLEN;
                } 
            }
            else {
                cipherText[ndx] = plainText[ndx];
            }
            if (keyCur > keyLen) {
                keyCur = 0;
            }
        }
        fwrite(cipherText, 1, readBytes, args->outFile);
    } while (readBytes == BUFFERSIZE);
}

//Copied over the encipher function, just modify it to decipher
//Actually, verify that the encipher works
/*
void decipher(arguments *args) {
    char plainText[BUFFERSIZE]; 
    char cipherText[BUFFERSIZE]; 
    int readBytes;// = fread(plainText, 1, BUFFERSIZE, args->inFile); 
    int keyLen = strlen(args->key);
    int keyCur = 0;
    
    do {
        readBytes = fread(plainText, 1, BUFFERSIZE, args->inFile); 
        for (ndx = 0; ndx < readBytes; ndx++) {
            if ('a' <= plainText[ndx] && 'Z' >= plainText[ndx]) {
                cipherText[ndx] = toUpper(plainText[ndx]); 
                cipherText[ndx] += args->key[keyCur++];
                if (cipherText[ndx] > 'Z') {
                    cipherText[ndx] -= ALPHABETLEN;
                } 
            }
            else {
                cipherText[ndx] = plainText[ndx];
            }
            if (keyCur > keyLen) {
                keyCur = 0;
            }
        }
        fwrite(cipherText, 1, readBytes, args->outFile);
    } while (readBytes == BUFFERSIZE);
}
*/

void checkArgs(int argc, char **argv, arguments *args) {
    int cnt = 1;

    while (cnt < argc) {
        if (strcmp((argv)[cur], "-v") == 0) {
            args->verbose = 1;
            printf("Verbosity Engaged\n");
        }
        else if (strcmp((argv)[cur], "-d") == 0) {
            args->decipher = 1;
            if (args->verbose) {
                printf("Deciphering mode\n"); 
            }
        }
        else if (args->key[0] == 0) {
            strcpy(args->key, (argv)[cur]);
            for (int i = 0; i < strlen(args->key); i++) {
                args->key[i] = toupper(args->key[i]);
                if (args->key[i] < 'A' || args->key[i] > 'Z') {
                    fprintf(stderr, "Invalid key, please use only latin numerals");
                    exit(1);
                }
            }
        }
        else if (args->inFile == NULL) {
            args->inFile = fopen(argv[cur], "r"); 
        }
        else {
            args->outFile = fopen(argv[cur], "w+"); 
        }
    }

    if (args->key == NULL) {
        fprintf(stderr, "Key not defined");
        exit(1);
    }
}

