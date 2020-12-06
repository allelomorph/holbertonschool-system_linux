#include "hls.h"
#include "flags.h"

/**
 * addListNode - adds a new node at the beginning of a file_list_t list
 * @head: double pointer to first member of a file_list_t list
 * @f_name: name of file corresponding to this position in the list
 * @f_stat: stat struct containing information about the file named
 * Return: address of the new element, or NULL if it failed
 */
file_list_t *addListNode(file_list_t **head, char *filename, char *path, struct stat st)
{
	file_list_t *new;
	char *rl_buf = NULL;
	size_t rl_bufSize = 256;

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
		readlink(filename, rl_buf, rl_bufSize);
		new->f_slnk = _strcopy(rl_buf);
	}
	else
		new->f_slnk = NULL;
	new->f_path = _strcopy(path);
	if (longFormat || fileSizeSort || modTimeSort)
		new->f_stat = statCopy(st);
	else
		new->f_stat = NULL;
	new->dir_files = NULL;
	new->prev = NULL;

	/* insert at head of list */
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

/**
 * parseArgs - parses command line arguments and populates linked lists of
 * files and directories accordingly
 * @argc: argument count passed from main
 * @argv: array of args passed from main
 * @file_list: pointer to head of a doubly linked list of file info structs
 * @dir_list: pointer to head of a doubly linked list of directory info structs
 */
void parseArgs(int argc, char *argv[], file_list_t **file_list,
	       file_list_t **dir_list)
{
	struct stat file_stat;
	int i, errno;
	bool nonFlagArgs = false;

	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] != '-')
		{
			nonFlagArgs = true;
			if (lstat(argv[i], &file_stat) == 0) /* read success */
			{
				if (S_ISDIR(file_stat.st_mode))
					addListNode(dir_list, argv[i], argv[i], file_stat);
				else
					addListNode(file_list, argv[i], argv[i], file_stat);
			}
			else
				fileError(argv[i]);
		}
	}

	/* if no args, or only flag args, defaults to `.` contents */
	if (argc == 1 || !nonFlagArgs)
	{
		if (lstat(".", &file_stat) == 0) /* read success */
			addListNode(dir_list, ".", ".", file_stat);
		else
			fileError(".");
	}
}

/**
 * testPrintFlags - easy reference for flag toggles
 */
void testPrintFlags(void)
{
	printf("singleColumn   = %s\n", singleColumn ? "true" : "false");
	printf("allFiles       = %s\n", allFiles ? "true" : "false");
	printf("almostAllFiles = %s\n", almostAllFiles ? "true" : "false");
	printf("longFormat     = %s\n", longFormat ? "true" : "false");
	printf("reverseOrder   = %s\n", reverseOrder ? "true" : "false");
	printf("fileSizeSort   = %s\n", fileSizeSort ? "true" : "false");
	printf("modTimeSort    = %s\n", modTimeSort ? "true" : "false");
	printf("Recursive      = %s\n", Recursive ? "true" : "false");
}

/**
 * testPrintList - reference printing for doubly linked lists
 * @head: double pointer to first member of a file_list_t list
 * Return: amount of nodes printed
 */
size_t testPrintList(file_list_t *head)
{
	const file_list_t *temp = head;
	size_t nodes;

	for (nodes = 0; temp; nodes++)
	{
		printf("index: %lu node @: %p\n", nodes, (void *)temp);
		printf("\tf_name: %s\n", temp->f_name);
		printf("\tf_slnk: %s\n", temp->f_slnk);
		printf("\tf_path: %s\n", temp->f_path);
		printf("\tf_stat @: %p\n", (void *)temp->f_stat);
		printf("\tdir_files @: %p\n", (void *)temp->dir_files);
		if (temp->dir_files)
		{
			printf("\n");
			testPrintList(temp->dir_files);
			printf("\n");
		}
		printf("\tprev @: %p\n", (void *)temp->prev);
		printf("\tnext @: %p\n", (void *)temp->next);
		temp = temp->next;
	}
	return (nodes);
}
