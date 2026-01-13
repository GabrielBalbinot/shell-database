#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "common.h"
#include "parse.h"

void list_employees(struct dbheader_t *dbhdr, struct employee_t *employees) {

}

int add_employee(struct dbheader_t *header, struct employee_t **employees, char *addstring) {
    
    if (header == NULL || employees == NULL || *employees == NULL || addstring == NULL) 
        return STATUS_ERROR;
    
    char *name = strtok(addstring, ",");
    if (name == NULL) return STATUS_ERROR;
    
    char *addr = strtok(NULL, ",");
    if (addr == NULL) return STATUS_ERROR;
    
    char *hours = strtok(NULL, ",");
    if (hours == NULL) return STATUS_ERROR;
    
    struct employee_t *e = *employees;
    e = realloc(e, sizeof(struct employee_t) * header->count+1);
    if (e == NULL) {
        printf("Malloc failed\n");
        return STATUS_ERROR;
    }
    
    header->count++;
    
    strncpy(e[header->count-1].name, name, sizeof(e[header->count-1].name));
    strncpy(e[header->count-1].address, addr, sizeof(e[header->count-1].address));
    
    e[header->count-1].hours = atoi(hours);
    
    *employees = e;
    
    return STATUS_SUCCESS;
    
}

int read_employees(FILE *fd, struct dbheader_t *header, struct employee_t **employeesOut) {
    if (fd == NULL) {
        printf("Bad FD from user\n");
        return STATUS_ERROR;
    }
    
    int count = header->count;
    
    struct employee_t *employees = calloc(count, sizeof(struct employee_t));
    
    if (employees == NULL) {
        
        printf("Malloc failed\n");
        return STATUS_ERROR;
        
    }
    
    fread(employees, sizeof(struct employee_t), count, fd);
    
    for (int i=0; i < count; i++) {
        
        employees[i].hours = ntohl(employees[i].hours);
        
    }
    
    *employeesOut = employees;
    
    return STATUS_SUCCESS;
}

int output_file(FILE *fd, struct dbheader_t *header, struct employee_t *employees) {
    
    if (fd == NULL) {
        printf("Bad FD from user\n");
        return STATUS_ERROR;
    }
    
    int count = header->count;
    
    header->version = htons(header->version);
    header->count = htons(header->count);
    header->magic = htonl(header->magic);
    header->filesize = htonl(sizeof(struct dbheader_t) + sizeof(struct employee_t) * count);
    
    fseek(fd, 0, SEEK_SET);
    fwrite(header, sizeof(struct dbheader_t), 1, fd);
    fseek(fd, sizeof(struct dbheader_t), SEEK_SET);

    for (int i=0; i < count; i++) {
        
        employees[i].hours = htonl(employees[i].hours);
        fwrite(&employees[i], sizeof(struct employee_t), 1, fd);
        
    }
    
    return STATUS_SUCCESS;
    
}	

int validate_db_header(FILE *fd, struct dbheader_t **headerOut) {
    
    if (fd == NULL) {
        printf("Bad FD from user\n");
        return STATUS_ERROR;
    }
    
    struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
    
    if (header == NULL) {
        printf("Malloc failed to create header\n");
        return STATUS_ERROR;
    }
    
    int size = 0;
    
    if (fread(header, 1, sizeof(struct dbheader_t), fd) != sizeof(struct dbheader_t)) {
        perror("fread");
        free(header);
        return STATUS_ERROR;
    }
    
    header->version = ntohs(header->version);
    header->count = ntohs(header->count);
    header->magic = ntohl(header->magic);
    header->filesize = ntohl(header->filesize);
    
    if (header->magic != HEADER_MAGIC) {
        printf("Improper head magic\n");
        free(header);
        return STATUS_ERROR;
    }
    
    if (header->version != 1) {
        printf("Improper header version\n");
        free(header);
        return STATUS_ERROR;
    }
    
    struct stat dbstat = {0};
    fstat(fileno(fd), &dbstat);
    if (header->filesize != dbstat.st_size) {
        printf("Corrupted database\n");
        free(header);
        return STATUS_ERROR;
    }
    
    *headerOut = header;
    
    
}

int create_db_header(FILE  *fd, struct dbheader_t **headerOut) {
	
	struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
	
	if (header == NULL) {
	    printf("Malloc failed to create header\n");
	    return STATUS_ERROR;
	}
	
	header->version = 0x1;
	header->count = 0;
	header->magic = HEADER_MAGIC;
	header->filesize = sizeof(struct dbheader_t);
	
	*headerOut = header;
	
	return STATUS_SUCCESS;
	
}


