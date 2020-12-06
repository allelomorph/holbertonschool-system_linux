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
 * @f_path: full path relative to working directory
 * @f_stat: pointer to stat struct with lstat output
 * @dir_files: head of file_list_t of directory contents
 * @prev: pointer to next node
 * @next: pointer to previous node
 */
typedef struct file_list_s
{
	const char *f_name;
	const char *f_slnk;
	const char *f_path;
	const struct stat *f_stat;
	struct file_list_s *dir_files;
	struct file_list_s *prev;
	struct file_list_s *next;
} file_list_t;

/* make file struct f_path be full path: temp/folder1, f_name: folder1 */
/* if cmdLineArgs, then f_name and f_path are copies of the same string */

/*
* hls_main.c *
void setFlags(char *flags);
void parseArgs(int argc, char *argv[], file_list_t **file_list,
	       file_list_t **dir_list);
void printFileList(file_list_t *head);
void parseDirs(file_list_t *dir_list_head);
void printDirs(file_list_t *dir_list_head);

* hls_printing.c *
int longFormatPrint(file_list_t *node);
char *modeString(mode_t mode);
char *dateTimeString(time_t time);
bool displayAllowed(const char *filename);

* hls_lists.c *
file_list_t *addListNode(file_list_t **head, char *f_name, struct stat f_stat);
void freeList(file_list_t *head);
void deleteListNode(file_list_t *node);

* hls_errors.c *
void accessError(const char *file);
* perms error subr? *

* hls_misc.c *
struct stat *statCopy(struct stat st);
char *_strcopy(char *string);
bool stringExactMatch(const char *s1, char *s2);

* hls_tests.c *
void testPrintFlags(void);
size_t testPrintList(file_list_t *head);
*/

/* helpers1.c */
void setFlags(char *flags);
struct stat *statCopy(struct stat st);
char *_strcopy(char *string);
void printFileList(file_list_t *head, bool cmdLineArgs);
void deleteListNode(file_list_t *node);

/* helpers2.c */
file_list_t *addListNode(file_list_t **head, char *filename, char *path,
			 struct stat f_stat);
void freeList(file_list_t *head);
int parseArgs(int argc, char *argv[], file_list_t **file_list,
	       file_list_t **dir_list);
void testPrintFlags(void);
size_t testPrintList(file_list_t *head);

/* helpers3.c */
char *modeString(mode_t mode);
char *dateTimeString(time_t time);
int longFormatPrint(file_list_t *node);
bool stringExactMatch(const char *s1, char *s2);

/* helpers4.c */
void parseDirs(file_list_t *dir_list_head, bool cmdLineArgs);
void printDirs(file_list_t *dir_list_head, bool cmdLineArgs,
	       int nonFlagArgs);
bool displayAllowed(const char *filename);
bool dirParseAllowed(const char *dirname, bool cmdLineArgs);
void fileError(const char *file);

#endif /* HLS_H */
