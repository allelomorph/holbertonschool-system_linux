#include "hls.h"
#include "flags.h"

/**
 * setFlags - sets global booleans based on toggles in command line args
 * @flags: single arg containing flags
 */
void setFlags(char *flags)
{
	int i;

	for (i = 1; flags[i]; i++)
	{
		switch (flags[i])
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
			exit(2);
		}
	}
}

/**
 * statCopy - duplicates a stat struct and its contents
 * @st: original stat struct to copy
 * Return: pointer to copied struct, or NULL on failure
 */
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

	return (new);
}

/**
 * _strcopy - copies a string
 * @string: original string
 * Return: copied string, or NULL on failure
 */
char *_strcopy(char *string)
{
	int i, len = 0;
	char *copy = NULL;

	if (!string)
		return (NULL);

	/* measure length */
	for (i = 0; string[i]; i++)
		len++;

	/* new array */
	copy = malloc(sizeof(char) * (len + 1));
	if (!copy)
	{
		fprintf(stderr, "_strcopy: Can't malloc\n");
		return (NULL);
	}

	/* copy */
	for (i = 0; i <= len; i++)
		copy[i] = string[i];

	return (copy);
}

/**
 * printFileList - prints each member of a file_list_t list, according to flags
 * @head: pointer to first member of a file_list_t list
 * @cmdLineArgs: indicates whether or not acting upon file lists created
 * from initial args
 */
void printFileList(file_list_t *head, bool cmdLineArgs)
{
	file_list_t *temp = head;
	bool firstInList = true;

	/* if reverse flag is on, advance to tail for reversed traversal */
	if (reverseOrder)
		while (temp->next)
			temp = temp->next;

	while (temp)
	{
		if (cmdLineArgs || displayAllowed(temp->f_name))
		{
			if (!firstInList)
			{
				if (singleColumn || longFormat)
					printf("\n");
				else
					printf("  ");
			}

			if (longFormat)
				longFormatPrint(temp);
			else
				printf("%s", temp->f_name);

			firstInList = false;
		}

		/* next node */
		if (reverseOrder)
			temp = temp->prev;
		else
			temp = temp->next;

		/* final newline */
		if (!temp)
			printf("\n");
	}
}

/**
 * deleteListNode - removes a node from a doubly linked list
 * @node: pointer to a member of a file_list_t list
 */
void deleteListNode(file_list_t *node)
{
	file_list_t *temp, *bef_cut, *aft_cut;

	if (!node)
		return;

	temp = node;

	bef_cut = temp->prev;
	aft_cut = temp->next;
	if (bef_cut)
		bef_cut->next = aft_cut;
	if (aft_cut)
		aft_cut->prev = bef_cut;

	if (temp->f_name)
		free((char *)temp->f_name);
	if (temp->f_slnk)
		free((char *)temp->f_slnk);
	if (temp->f_stat)
		free((struct stat *)temp->f_stat);
	if (temp->dir_files)
		freeList(temp->dir_files);

	free(temp);
}
