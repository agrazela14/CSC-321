#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
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
    args.inFile = STDIN_FILENO;
    args.outFile = STDOUT_FILENO;
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
    int ndx;
    
    do {
        readBytes = read(args->inFile, plainText, BUFFERSIZE); 
        for (ndx = 0; ndx < readBytes; ndx++) {
            plainText[ndx] = toupper(plainText[ndx]);
            if ('A' <= plainText[ndx] && 'Z' >= plainText[ndx]) {
                cipherText[ndx] = plainText[ndx]; //implicit def. of toupper, for whatever reason 
                cipherText[ndx] += args->key[keyCur++] - 'A';
                if (cipherText[ndx] > 'Z') {
                    cipherText[ndx] -= ALPHABETLEN;
                } 
            }
            else {
                cipherText[ndx] = plainText[ndx];
            }
            if (keyCur >= keyLen) {
                keyCur = 0;
            }
        }
        write(args->outFile, cipherText, readBytes);
    } while (readBytes == BUFFERSIZE);
}

//Copied over the encipher function, just modify it to decipher
//Actually, verify that the encipher works
void decipher(arguments *args) {
    char plainText[BUFFERSIZE]; 
    char cipherText[BUFFERSIZE]; 
    int readBytes;// = fread(plainText, 1, BUFFSIZE, args->inFile); 
    int keyLen = strlen(args->key);
    int keyCur = 0;
    int ndx;
    
    do {
        readBytes = read(args->inFile, cipherText, BUFFERSIZE); 
        for (ndx = 0; ndx < readBytes; ndx++) {
            cipherText[ndx] = toupper(cipherText[ndx]);
            if ('A' <= cipherText[ndx] && 'Z' >= cipherText[ndx]) {
                plainText[ndx] = cipherText[ndx]; //implicit def. of toupper, for whatever reason 
                plainText[ndx] -= args->key[keyCur++] - 'A';
                if (plainText[ndx] < 'A') {
                    plainText[ndx] += ALPHABETLEN;
                } 
            }
            else {
                plainText[ndx] = cipherText[ndx];
            }
            if (keyCur >= keyLen) {
                keyCur = 0;
            }
        }
        write(args->outFile, plainText, readBytes);
    } while (readBytes == BUFFERSIZE);
}

void checkArgs(int argc, char **argv, arguments *args) {
    int cnt = 1;

    while (cnt < argc) {
        if (strcmp((argv)[cnt], "-v") == 0) {
            args->verbose = 1;
            printf("Verbosity Engaged\n");
        }
        else if (strcmp((argv)[cnt], "-d") == 0) {
            args->decipher = 1;
            if (args->verbose) {
                printf("Deciphering mode\n"); 
            }
        }
        else if (args->key[0] == 0) {
            strcpy(args->key, (argv)[cnt]);
            for (int i = 0; i < strlen(args->key); i++) {
                args->key[i] = toupper(args->key[i]);
                if (args->key[i] < 'A' || args->key[i] > 'Z') {
                    fprintf(stderr, "Invalid key, please use only latin numerals\n");
                    exit(1);
                }
            }
        }
        else if (args->inFile == STDIN_FILENO) {
            args->inFile = open(argv[cnt], O_RDONLY); 
        }
        else {
            args->outFile = open(argv[cnt], O_RDWR | O_TRUNC | O_CREAT, S_IRWXU); 
        }
        cnt++;
    }

    if (args->key == NULL) {
        fprintf(stderr, "Key not defined\n");
        exit(1);
    }
}

