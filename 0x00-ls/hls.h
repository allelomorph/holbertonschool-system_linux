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
 * struct coll_elem_s - collation element for a given char
 * note: simplifed from Unicode standard in two ways:
 * - unsigned char vs unsigned short int
 * - two weights instead of three
 *
 * @weights: array of unsigned values to drive sorting at 3 levels
 * @variable: if true, special collation behavior at level 3
 */
typedef struct coll_elem_s {
	unsigned char weights[2];
	bool variable;
} coll_elem_t;

/**
 * struct coll_key_s - doubly linked list node containing collation key
 *
 * @n: one of two weighted values from the collation element for a char
 * @prev: pointer to next node
 * @next: pointer to previous node
 */
typedef struct coll_key_s {
	unsigned char n;
	struct coll_key_s *prev;
	struct coll_key_s *next;
} coll_key_t;

/**
 * struct file_list_s - doubly linked list node containing file/dir info
 *
 * @f_name: file or dir name
 * @f_ckey: SLL of file name collation key for sorting
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
	coll_key_t *f_ckey;
	const char *f_slnk;
	const char *f_path;
	const struct stat *f_stat;
	struct file_list_s *dir_files;
	struct file_list_s *prev;
	struct file_list_s *next;
} file_list_t;

/* make file struct f_path be full path: temp/folder1, f_name: folder1 */
/* if cmdLineArgs, then f_name and f_path are copies of the same string */

/* hls.c */
void setFlags(char *flags);
/* int main(int argc, char *argv[]) */

/* collation_keys.c */
int _strcoll(coll_key_t *ckey_s1, coll_key_t *ckey_s2);
coll_key_t *buildCollKey(const char *s);
coll_key_t *addCollKeyNode(coll_key_t **head);
void freeCollKey(coll_key_t **head);
void setCollElem(coll_elem_t *elem, char c);

/* file_lists.c */
void deleteListNode(file_list_t *node);
struct stat *statCopy(struct stat st);
file_list_t *addListNode(file_list_t **head, char *filename, char *path,
			 struct stat f_stat);
void freeList(file_list_t *head);
void reverseList(file_list_t **head);

/* long_format_printing.c */
char *modeString(mode_t mode);
char *dateTimeString(time_t time);
int longFormatPrint(file_list_t *node);

/* parsing.c */
int parseArgs(int argc, char *argv[], file_list_t **file_list,
	       file_list_t **dir_list);
void parseDirs(file_list_t *dir_list_head, bool cmdLineArgs);
bool dirParseAllowed(const char *dirname, bool cmdLineArgs);

/* printing.c */
bool displayAllowed(const char *filename);
void printFileList(file_list_t *head, bool cmdLineArgs);
void printDirs(file_list_t *dir_list_head, bool cmdLineArgs,
	       int nonFlagArgs);
void fileError(const char *file);

/* sorting.c */
int criteriaSort(file_list_t *node1, file_list_t *node2);
void insertion_sort_list(file_list_t **list);
void dll_adj_swap(file_list_t **list, file_list_t *left, file_list_t *right);
void cocktail_sort_list(file_list_t **list);
void sortDirs(file_list_t *dir_list);

/* string_utils */
bool stringExactMatch(const char *s1, char *s2);
char *_strcopy(char *string);
unsigned int _strlen(const char *s);

/* test_printing.c */
void testPrintFlags(void);
size_t testPrintList(file_list_t *head);


#endif /* HLS_H */


/* no hidden file screen on files from args, only on files from dirs */
