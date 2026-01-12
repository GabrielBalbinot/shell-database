#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "file.h"
#include "common.h"


int create_db_file(char *filename, FILE **fileOut) {

    FILE *file = fopen(filename, "r");

    if (file != NULL) {

        fclose(file);
        printf("File already exists\n");
        return STATUS_ERROR;

    }
    
    file = fopen(filename, "w");

    if (file == NULL) {
        perror("fopen");
        return STATUS_ERROR;
    }
    
    *fileOut = file;

    return STATUS_SUCCESS;

}

int open_db_file(char *filename, FILE **fileOut) {

    FILE *file = fopen(filename, "rb+");

    if (file == NULL) {

        perror("fopen");
        return STATUS_ERROR;

    }

    *fileOut = file;
    return STATUS_SUCCESS;

}


