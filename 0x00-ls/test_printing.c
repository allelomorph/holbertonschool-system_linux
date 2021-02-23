#include "hls.h"
#include "flags.h"


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
