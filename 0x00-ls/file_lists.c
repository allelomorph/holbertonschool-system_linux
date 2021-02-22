#include "hls.h"
#include "flags.h"


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
 * addListNode - adds a new node at the beginning of a file_list_t list
 * @head: double pointer to first member of a file_list_t list
 * @filename: name of file corresponding to this position in the list
 * @path: full path of file relative to working directory
 * @st: stat struct containing information about the file named
 * Return: address of the new element, or NULL if it failed
 */
file_list_t *addListNode(file_list_t **head, char *filename, char *path,
			 struct stat st)
{
	file_list_t *new;
	char *rl_buf = NULL;
	size_t rl_bufSize = 256;
	ssize_t read_bytes = 0;

	rl_buf = malloc(rl_bufSize);
	new = malloc(sizeof(file_list_t));
	if (!new || !rl_buf)
	{
		fprintf(stderr, "addListNode: Can't malloc\n");
		return (NULL);
	}

	/* init contents */
	new->f_name = _strcopy(filename);
	if (S_ISLNK(st.st_mode))
	{
		read_bytes = readlink(path, rl_buf, rl_bufSize - 1);
		if (read_bytes == -1)
			fileError(path);
		else
		{
			/* readlink does not automatically NULL-terminate */
			rl_buf[read_bytes] = '\0';
			new->f_slnk = _strcopy(rl_buf);
		}
	}
	else
		new->f_slnk = NULL;
	new->f_path = _strcopy(path);
	if (longFormat || fileSizeSort || modTimeSort)
		new->f_stat = statCopy(st);
	else
		new->f_stat = NULL;
	new->dir_files = NULL;

	/* insert at head of list */
	new->prev = NULL;
	new->next = *head;
	if (*head)
		(*head)->prev = new;
	*head = new;

	free(rl_buf);
	return (new);
}


/**
 * freeList - frees all node members and nodes of a file_list_t list
 * @head: double pointer to first member of a file_list_t list
 */
void freeList(file_list_t *head)
{
	file_list_t *temp;

	while (head)
	{
		temp = head;
		head = head->next;

		/* cast each to non-const to free */
		if (temp->f_name)
			free((char *)temp->f_name);
		if (temp->f_slnk)
			free((char *)temp->f_slnk);
		if (temp->f_path)
			free((char *)temp->f_path);
		if (temp->f_stat)
			free((struct stat *)temp->f_stat);

		/* recurses to clear list of dir contents */
		if (temp->dir_files)
			freeList(temp->dir_files);

		free(temp);
	}
}


void reverseList(file_list_t **head)
{
	file_list_t *temp, *swap;

	temp = *head;

	while (temp)
	{
		swap = temp->next;
		temp->next = temp->prev;
		temp->prev = swap;

		if (!temp->prev)
		{
			*head = temp;
			break;
		}

		temp = swap;
	}
}
