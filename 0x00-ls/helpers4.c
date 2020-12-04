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

void parseDirs(file_list_t *dir_list_head)
{
	DIR *dir;
	struct dirent *read;
	file_list_t *file_list_head = NULL, *temp = dir_list_head;
	struct stat file_stat;
	char *path = NULL;

	path = malloc(sizeof(char) * 256);

	while (temp)
	{
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

		dir_list_head->dir_files = file_list_head;

		temp = temp->next;
	}
	free(path);
}

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
