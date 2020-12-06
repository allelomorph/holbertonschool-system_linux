#include "hls.h"
#include "flags.h"

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
		temp = Recursive ? NULL : temp->next;
	}
	free(path);
}


/**
 * printDirs - prints any directories found in a file_list_t list,
 * and their contents
 * @dir_list_head: head of doubly linked list of directory profiles
 * @cmdLineArgs: indicates whether dirs in list were named as args
 * @fileArgsEmpty: indicates whether or not command line args contained files
 */
void printDirs(file_list_t *dir_list_head, bool cmdLineArgs,
	       int nonFlagArgs)
{
	file_list_t *temp = dir_list_head;
	bool onlyOneDir = false, firstDir = true;

	/* special case of one dir arg and no other nonflag args */
	if (nonFlagArgs < 2 && !(temp->next))
		onlyOneDir = true;

	/* first dir in list omits leading newline */
	if (!onlyOneDir && !(temp->prev))
		firstDir = true;

	/* if reverse flag is on, advance temp to tail for reversed traversal */
	if (reverseOrder)
		while (temp->next)
			temp = temp->next;

	while (temp)
	{
		if (dirParseAllowed(temp->f_name, cmdLineArgs)
		    && temp->dir_files)
		{
			/*
			printf("firstDir: %s\n", firstDir ? "true" : "false");
			*/
			if (!onlyOneDir && !firstDir)
				printf("\n");

			/* recusive mode prints dir name even if only one arg */
			if (Recursive || !onlyOneDir)
				printf("%s:\n", temp->f_path);

			cmdLineArgs = false;
			printFileList(temp->dir_files, cmdLineArgs);

			if (Recursive)
				printDirs(temp->dir_files, cmdLineArgs,
					  nonFlagArgs);

			firstDir = false;
		}
		if (reverseOrder)
			temp = temp->prev;
		else
			temp = temp->next;
	}
}

/**
 * displayAllowed - checks filename against active flags to determine
 * if it can be displayed
 * @filename: name of file to be diplayed next
 * Return: boolean giving permission to print
 */
bool displayAllowed(const char *filename)
{
	if (stringExactMatch(filename, ".") ||
	    stringExactMatch(filename, ".."))
		return (allFiles);
	else if (filename[0] == '.')
		return (almostAllFiles || allFiles);

	return (true);
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

/**
 * fileError - prints errors to stderr based on errno value, sets exit code
 * @file: name of file that caused access error
 */
void fileError(const char *file)
{
	switch (errno)
	{
	case ENOENT:
		/* file/dir missing or bad name */
		fprintf(stderr, "hls: cannot access ");
		break;
	case EACCES:
		/* stock ls prints files even with locked perms */
		/* but locked dirs will throw an error */
		fprintf(stderr, "hls: cannot open directory ");
		break;
	default:
		fprintf(stderr, "hls: unknown error ");
		break;
	}

	perror(file);
	errno = 0;
	exitCode = 2;
}
