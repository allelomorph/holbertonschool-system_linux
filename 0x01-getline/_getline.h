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

/* macros */
#define READ_SIZE 1024

/* structs */
/**
 * struct fd_profile_s - node in a singly linked list of fd profiles
 *
 * @fd: file descriptor to read
 * @read_buf: buffer of READ_SIZE bytes to read into
 * @line_start: starting index of next \n delimited line
 * @next: pointer to next node in list
 */
typedef struct fd_profile_s
{
	int fd;
	char *read_buf;
	size_t line_start;
	struct fd_profile_s *next;
} fd_profile_t;

/* functions */
/* (up to 5 helpers allowed to _getline() */

void free_profiles(fd_profile_t **head);
fd_profile_t *add_profile(fd_profile_t  **head, int fd);
char *join_line(char **unfinished_line, char **new_line,
		size_t unfinished_line_len, size_t line_len);
char *copy_line(char *read_buf, size_t *line_start, bool *overflow);
char *buff_load(int fd, char *read_buf, size_t line_start);

char *_getline(const int fd);

#endif /* _GETLINE_H */
