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

/**
   UOPDATE UPDATE UPDATE!!
 * add_dnodeint - adds a new node at the beginning of a dlistint_t list
 * @head: double pointer to first member of a dlistint_t list
 * @n: int value stored in new node
 * Return: address of the new element, or NULL if it failed
 */

file_list_t *addListNode(file_list_t **head, char *f_name, struct stat f_stat)
{
        file_list_t *new;

	new = malloc(sizeof(file_list_t));
	if (!new)
	{
		fprintf(stderr, "addListNode: Can't malloc\n");
		return (NULL);
	}

        new->f_name = _strcopy(f_name);
	/* f_stat can stay empty pointer unless -l -S or -t is on */
	printf("longFormat: %i fileSizeSort: %i modTimeSort: %i\n", longFormat, fileSizeSort, modTimeSort);
	if (longFormat || fileSizeSort || modTimeSort)
		new->f_stat = statCopy(f_stat);
	else
		new->f_stat = NULL;
	new->prev = NULL;
	new->next = *head;

	if (*head)
		(*head)->prev = new;

	*head = new;

	return (new);
}

/* parseArgs(argv)
   runs setFlags on flag args
   lstat remaining args
   throw errors to stderr (errs in arg order not sorted order)
   use remainder to populate one list/array of general files,
   and one of dirs
*/
void parseArgs(int argc, char *argv[], file_list_t **file_list,
	       file_list_t **dir_list)
{
	struct stat file_stat;
	int i;
	int errno;

	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
			setFlags(argv[i]);
	}

	/* need to store destinations of soft links */
	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] != '-')
		{
			if (lstat(argv[i], &file_stat) == 0) /* read success */
			{
				if (S_ISDIR(file_stat.st_mode))
					addListNode(dir_list, argv[i], file_stat);
				/*
				  else if (S_ISLNK(file_stat.st_mode))
				  add link dest string to list node
				*/
				else
					addListNode(file_list, argv[i], file_stat);
			}
			else
			{
				fprintf(stderr, "parseArgs: cannot access ");
				perror(argv[i]);
			}
		}
	}

	/* if both lists are empty, then either no args, */
        /* or only flag args were passed, defaults to `.` */
	/* separate func defaultDir? */
	if (argc == 1 || (!file_list && !dir_list))
	{
		if (lstat(".", &file_stat) == 0) /* read success */
			addListNode(dir_list, ".", file_stat);
		else
		{
			fprintf(stderr, "parseArgs: cannot access ");
			perror(".");
		}
	}
}

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

size_t testPrintList(file_list_t *head)
{
	const file_list_t *temp = head;
	size_t nodes;

	for (nodes = 0; temp; nodes++)
	{
		printf("index: %lu node address: %p\n", nodes, (void *)temp);
		printf("\tf_name: %s\n", temp->f_name);
		printf("\tf_stat @: %p\n", (void *)temp->f_stat);
		printf("\tprev @: %p\n", (void *)temp->prev);
		printf("\tnext @: %p\n", (void *)temp->next);
		temp = temp->next;
	}
	return (nodes);
}
