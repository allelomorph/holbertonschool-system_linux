#include "hls.h"

/* flags */
extern bool singleColumn;
extern bool allFiles;
extern bool almostAllFiles;
extern bool longFormat;
extern bool reverseOrder;
extern bool fileSizeSort;
extern bool modTimeSort;
extern bool Recursive;

int setFlags(char *flags)
{
	int i;

	for(i = 1; flags[i]; i++)
	{
		switch(flags[i])
		{
		case '1':
			singleColumn   = true;
			break;
		case 'a':
			allFiles       = true;
			break;
		case 'A':
			almostAllFiles = true;
			break;
		case 'l':
			longFormat     = true;
			break;
		case 'r':
			reverseOrder   = true;
			break;
		case 'S':
			fileSizeSort   = true;
			break;
		case 't':
			modTimeSort    = true;
			break;
		case 'R':
			Recursive      = true;
			break;
		default:
			fprintf(stderr, "hls: invalid option -- '%c'\n", flags[i]);
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

void readDirectory(char *dirName)
{
	DIR *dir;
	struct dirent *read;
	/* struct stat *buf; */

	dir = opendir(dirName);

	while ((read = readdir(dir)) != NULL)
	{
		/* hidden files */
		if (read->d_name[0] == '.')
		{
			/* . and .. */
			if ((read->d_name[1] == '.' && !(read->d_name[2]))
			    || !(read->d_name[1]))
			{
				if (allFiles)
					printf("%s  ", read->d_name);
			}
			/* user-created hidden */
			else if (almostAllFiles)
				printf("%s  ", read->d_name);
		}
		/* normal files */
		else
			printf("%s  ", read->d_name);
	}
	printf("\n");
	closedir(dir);
}


/* copies stat structs */
struct stat *statCopy(struct stat st)
{
	struct stat *new;

	new = malloc(sizeof(struct stat));
	if (!new)
	{
		fprintf(stderr, "statCopy: Can't malloc\n");
		return (NULL);
	}
	new->st_dev = st.st_dev;
	new->st_ino = st.st_ino;
	new->st_mode = st.st_mode;
	new->st_nlink = st.st_nlink;
	new->st_uid = st.st_uid;
	new->st_gid = st.st_gid;
	new->st_rdev = st.st_rdev;
	new->st_size = st.st_size;
	new->st_blksize = st.st_blksize;
	new->st_blocks = st.st_blocks;
	new->st_atime = st.st_atime;
	new->st_mtime = st.st_mtime;
	new->st_ctime = st.st_ctime;

	return new;
}

char *_strcopy(char *string)
{
	int i, len;
	char *copy;

	if (!string)
		return NULL;

	/* measure length */
	for(i = 0; string[i]; i++)
		len++;

	/* new array */
	copy = malloc(sizeof(char) * (len + 1));
	if (!copy)
	{
		fprintf(stderr, "_strcopy: Can't malloc\n");
		return (NULL);
	}

	/* copy */
	for(i = 0; string[i]; i++)
		copy[i] = string[i];

	return copy;
}
