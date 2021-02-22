#include "hls.h"
#include "flags.h"

/**
 * parseArgs - parses command line arguments and populates linked lists of
 * files and directories accordingly
 * @argc: argument count passed from main
 * @argv: array of args passed from main
 * @file_list: pointer to head of a doubly linked list of file info structs
 * @dir_list: pointer to head of a doubly linked list of directory info structs
 */
int parseArgs(int argc, char *argv[], file_list_t **file_list,
	       file_list_t **dir_list)
{
	struct stat file_stat;
	int i, errno;
	int nonFlagArgs = 0;

	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] != '-')
		{
			nonFlagArgs++;
			if (lstat(argv[i], &file_stat) == 0) /* read success */
			{
				if (S_ISDIR(file_stat.st_mode))
					addListNode(dir_list, argv[i],
						    argv[i], file_stat);
				else
					addListNode(file_list, argv[i],
						    argv[i], file_stat);
			}
			else
				fileError(argv[i]);
		}
	}

	/* lists are built by adding at the head, to be sorted later */
	/* but by default, args should be printed in arg order */
/*
	if (!fileSizeSort && !modTimeSort)
	{
		if (file_list)
			reverseList(file_list);
		if (dir_list)
			reverseList(dir_list);
	}
*/
	/* if no args, or only flag args, defaults to `.` contents */
	if (nonFlagArgs == 0)
	{
		if (lstat(".", &file_stat) == 0) /* read success */
			addListNode(dir_list, ".", ".", file_stat);
		else
			fileError(".");
	}

	return (nonFlagArgs);
}


/**
 * parseDirs - walks through a list of directories and populates their
 * `dir_files` list
 * @dir_list_head: head of doubly linked list of directory profiles
 * @cmdLineArgs: boolean indicating if directory was passed by user as arg
 */
void parseDirs(file_list_t *dir_list_head, bool cmdLineArgs)
{
	DIR *dir;
	struct dirent *read;
	file_list_t *file_list_head, *temp = dir_list_head;
	struct stat file_stat;
	char *path = NULL;

	path = malloc(sizeof(char) * 256);

	while (temp)
	{
		if (dirParseAllowed(temp->f_name, cmdLineArgs))
		{
			file_list_head = NULL;
			dir = opendir(temp->f_path);

			if (dir)
			{
				while ((read = readdir(dir)) != NULL)
				{
					sprintf(path, "%s/%s", temp->f_path,
						read->d_name);
					if (lstat(path, &file_stat) == 0)
						addListNode(&file_list_head,
							    read->d_name, path, file_stat);
					else
						fileError(path);
					if (S_ISDIR(file_stat.st_mode) && Recursive)
						parseDirs(file_list_head, false);
				}
				if (errno)
					fileError(temp->f_path);
				closedir(dir);
				temp->dir_files = file_list_head;
			}
			else
				fileError(temp->f_path);
		}
		/* when recursing, only parse current list node at this level */
		temp = (Recursive && !cmdLineArgs) ? NULL : temp->next;
	}
	free(path);
}


/**
 * dirParseAllowed - checks directory name against active flags to determine
 * if it needs to have its contents recorded by parseDirs
 * @dirname: name of directory to be parsed next
 * @cmdLineArgs: boolean indicating if directory was passed by user as arg
 * Return: boolean giving permission to store contents
 */
bool dirParseAllowed(const char *dirname, bool cmdLineArgs)
{
	/* normal dirs */
	if (!stringExactMatch(dirname, ".") &&
	    !stringExactMatch(dirname, ".."))
		return (true);

	/* . and .. should not be parsed to avoid inf. loops */
	/* but if either is passed explicity as an arg, it should be parsed */
	if ((stringExactMatch(dirname, ".") ||
	     stringExactMatch(dirname, "..")) && cmdLineArgs)
		return (true);

	return (false);
}
