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

/* consider making a `defaultDir` flag for when no args or only flag args */

/**
 * struct file_info_s - Doubly linked list node
 *
 * @c: char stored in the node
 * @prev: Pointer to the previous element of the list
 * @next: Pointer to the next element of the list
 */
typedef struct file_list_s
{
	const char *f_name;
	const struct stat *f_stat;
	struct file_list_s *prev;
	struct file_list_s *next;
} file_list_t;

/* helpers1.c */
int setFlags(char *flags);
void readDirectory(char *dirName);
struct stat *statCopy(struct stat st);
char *_strcopy(char *string);

/* helpers2.c */
file_list_t *addListNode(file_list_t **head, char *f_name, struct stat f_stat);
void parseArgs(int argc, char *argv[], file_list_t **file_list,
	       file_list_t **dir_list);
void testPrintFlags(void);
size_t testPrintList(file_list_t *head);

/* ALLOWED FUNCS
    opendir (man 3 opendir)
    readdir (man 3 readdir)
    closedir (man 3 closedir)
    exit (man 3 exit)
    free (man 3 free)
    lstat (man 2 lstat)
    malloc (man 3 malloc)
    perror (man 3 perror)
    write (man 2 write)
    printf (man 3 printf)
    sprintf (man 3 sprintf)
    fprintf (man 3 fprintf)
    readlink (man 2 readlink)
    ctime (man 3 ctime)
    getpwuid (man 3 getpwuid)
    getgrgid (man 3 getgrgid)
    errno (man 3 errno)
*/

/*
  struct stat {
    dev_t     st_dev;     * ID of device containing file *
    ino_t     st_ino;     * inode number *
    mode_t    st_mode;    * protection *
    nlink_t   st_nlink;   * number of hard links *
    uid_t     st_uid;     * user ID of owner *
    gid_t     st_gid;     * group ID of owner *
    dev_t     st_rdev;    * device ID (if special file) *
    off_t     st_size;    * total size, in bytes *
    blksize_t st_blksize; * blocksize for filesystem I/O *
    blkcnt_t  st_blocks;  * number of 512B blocks allocated *
    time_t    st_atime;   * time of last access *
    time_t    st_mtime;   * time of last modification *
    time_t    st_ctime;   * time of last status change *
  };
*/

/*
  ----tasks to define funcs:
  m sort of string list/array, by alpha but lowercase before upper
  -S file size, then islower sort of special struct list/array
  -t modify time, then islower sort of special struct list/array
  ?? -r reverse order array? or just traverse in reverse?
  m set boolean flags based on args
  m error line printing
  m error checking
*/

/* test print argv[] */
/*
  printf("argc: %i\n", argc);
  for(i = 0; i < argc; i++)
     printf("argv[%i]: %s\n", i, argv[i]);
*/

/* lstat macros
   S_ISREG(m)  is it a regular file?
   S_ISDIR(m)  directory?
   S_ISCHR(m)  character device?
   S_ISBLK(m)  block device?
   S_ISFIFO(m) FIFO (named pipe)?
   S_ISLNK(m)  symbolic link?  (Not in POSIX.1-1996.)
   S_ISSOCK(m) socket?  (Not in POSIX.1-1996.)
*/

/*
struct dirent {
	ino_t          d_ino;       * inode number *
	off_t          d_off;       * not an offset; see NOTES *
	unsigned short d_reclen;    * length of this record *
	unsigned char  d_type;      * type of file; not supported
				       by all filesystem types *
	char           d_name[256]; * filename *
}; */

	/* recursive will require
	      print non-dir arg files
	      for every dir in dir list:
	          print dir contents
	          make new dir lsit for subdirs
	          for every subdir in new list:
	               print subdir contents
	               make new dir list for subsubdirs....

	   recursion must handle all other flags (printing/sorting)
	*/

/* draft exerpt from main, mad obsolete by parseArgs */
#ifdef BALHBLAH
	char *rl_buf = NULL;
	size_t rl_bufSize = 256;

	rl_buf  = malloc(rl_bufSize);


	if (argc > 1)
	{
		for(i = 1; i < argc; i++)
		{
			if (argv[i][0] == '-')
				setFlags(argv[i]);
			else if (lstat(argv[i], &file_stat) == 0)
			{
				/* printf("%s accessible!!  ", argv[i]); */
				printf("%s mode: %li  \n", argv[i], (long)file_stat.st_mode);

				if (S_ISREG(file_stat.st_mode))
					printf("file: %s  ", argv[i]);
				if (S_ISDIR(file_stat.st_mode))
				{
					printf("\ndir: %s:\n", argv[i]);
					readDirectory(argv[i]);
				}
				if (S_ISLNK(file_stat.st_mode))
				{
					stat(argv[i], &soft_link);
					readlink(argv[i], rl_buf, rl_bufSize);
					printf("soft link: %s @ inode: %li -> linked file: %s @ inode: %li\n", argv[i], (long)file_stat.st_ino, rl_buf, (long)soft_link.st_ino);
				}
			}
			else
				perror(argv[i]);
		}
	}
	/* no args defaults to working dir */
	/* !!! what if no args save flags? currently that fails to engage readDir */
	else
		readDirectory(".");


	free(rl_buf);
#endif
