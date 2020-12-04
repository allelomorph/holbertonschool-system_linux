#include "hls.h"
#include "flags.h"

/**
 * parseDirs - walks through a list of directories and populates their
 * `dir_files` list
 * @dir_list_head: head of doubly linked list of directory profiles
 */
void parseDirs(file_list_t *dir_list_head)
{
	DIR *dir;
	struct dirent *read;
	file_list_t *file_list_head, *temp = dir_list_head;
	struct stat file_stat;
	char *path = NULL;

	path = malloc(sizeof(char) * 256);

	while (temp)
	{
		file_list_head = NULL;
		dir = opendir(temp->f_name);
		if (!dir)
			accessError(temp->f_name);
		while ((read = readdir(dir)) != NULL)
		{
			if (!stringExactMatch(read->d_name, ".") &&
			    !stringExactMatch(read->d_name, ".."))
			{
				sprintf(path, "%s/%s", temp->f_name, read->d_name);
				if (lstat(path, &file_stat) == 0)
				{
					addListNode(&file_list_head, read->d_name,
						    file_stat);
				}
				else
					accessError(path);
			}
		}
		if (errno == EBADF)
			accessError(temp->f_name);
		closedir(dir);

		temp->dir_files = file_list_head;
		temp = temp->next;
	}
	free(path);
}

/**
 * printDirs - print directories and their contents from a doubly linked list
 * @dir_list_head: head of doubly linked list of directory profiles
 */
void printDirs(file_list_t *dir_list_head)
{
	file_list_t *temp = dir_list_head;

	bool onlyOneDir = true;

	if (temp->next)
		onlyOneDir = false;

	/* if reverse flag is on, advance temp to tail for reversed traversal */
	if (reverseOrder)
		while (temp->next)
			temp = temp->next;

	while (temp)
	{
		if (!onlyOneDir)
			printf("%s:\n", temp->f_name);

		if (temp->dir_files)
			printFileList(temp->dir_files);

		/* newline before next dir, if not last */
		if ((!reverseOrder && temp->next) ||
		    (reverseOrder && temp->prev))
			printf("\n");

		if (reverseOrder)
			temp = temp->prev;
		else
			temp = temp->next;
	}
}
