#ifndef _GETLINE_H
#define _GETLINE_H

/* read, write req */
#include <unistd.h>
/* malloc, free req */
#include <stdlib.h>
/* strcpy, strncpy, strcat, strdup, memset, memcpy req */
#include <string.h>
/* for flags */
#include <stdbool.h>

/* perror() not allowed, for testing only (errno could be handled internally */
#include <stdio.h>
#include <errno.h>

#define READ_SIZE 1024

char *_getline(const int fd);

/* total of 5 helpers to _getline allowed */

#endif /* _GETLINE_H */
