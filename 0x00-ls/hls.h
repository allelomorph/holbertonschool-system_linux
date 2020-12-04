#ifndef HLS_H
#define HLS_H

/* opendir closedir lstat getpwuid getgrgid req */
#include <sys/types.h>
/* opendir readdir req */
#include <dirent.h>
/* lstat req */
#include <sys/stat.h>
/* lstat write readlink req */
#include <unistd.h>
/* exit free malloc req */
#include <stdlib.h>
/* perror printf sprintf fprintf req */
#include <stdio.h>
/* perror req */
#include <errno.h>
/* ctime req */
#include <time.h>
/* getpwuid req */
#include <pwd.h>
/* getgrgid req */
#include <grp.h>
/* errno req */
#include <errno.h>

/* for flags */
#include <stdbool.h>

/**
 * struct file_list_s - doubly linked list node
 *
 * @f_name: file or dir name
 * @f_slnk: symlink target name
 * @f_stat: pointer to stat struct with lstat output
 * @dir_files: if parsed dir, head of file list of contents
 * @prev: pointer to next node
 * @next: pointer to previous node
 */
typedef struct file_list_s
{
	const char *f_name;
	const char *f_slnk;
	const struct stat *f_stat;
	struct file_list_s *dir_files;
	struct file_list_s *prev;
	struct file_list_s *next;
} file_list_t;

/* helpers1.c */
void setFlags(char *flags);
struct stat *statCopy(struct stat st);
char *_strcopy(char *string);
void printFileList(file_list_t *head);
void deleteListNode(file_list_t *node);

/* helpers2.c */
file_list_t *addListNode(file_list_t **head, char *f_name, struct stat f_stat);
void freeList(file_list_t *head);
void parseArgs(int argc, char *argv[], file_list_t **file_list,
	       file_list_t **dir_list);
void testPrintFlags(void);
size_t testPrintList(file_list_t *head);

/* helpers3.c */
char *modeString(mode_t mode);
char *dateTimeString(time_t time);
int longFormatPrint(file_list_t *node);
void accessError(const char *file);
bool stringExactMatch(char *s1, char *s2);

/* helpers4.c */
void parseDirs(file_list_t *dir_list_head);
void printDirs(file_list_t *dir_list_head);

#endif /* HLS_H */
