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

/*
typedef struct fd_profile_s
{
	int fd;
	char *read_buf;
	size_t line_start;
	fd_profile_s *next;
} fd_profile_t;
*/

#define READ_SIZE 1024

char *copy_line(char *read_buf, size_t *line_start, bool *overflow);
char *buff_load(int fd, char *read_buf, size_t line_start);
char *_getline(const int fd);

/* total of 5 helpers to _getline allowed */

#endif /* _GETLINE_H */
