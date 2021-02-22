#include "hls.h"
#include "flags.h"


/**
 * printFileList - prints each member of a file_list_t list, according to flags
 * @head: pointer to first member of a file_list_t list
 * @cmdLineArgs: indicates whether or not acting upon file lists created
 * from initial args
 */
void printFileList(file_list_t *head, bool cmdLineArgs)
{
	file_list_t *temp = head;
	bool filesPrinted = false;
/*
	cocktail_sort_list(&head);
*/
	/* if reverse flag is on, advance to tail for reversed traversal */
	if (reverseOrder)
		while (temp->next)
			temp = temp->next;

	while (temp)
	{
		if (cmdLineArgs || displayAllowed(temp->f_name))
		{
			if (filesPrinted)
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

			filesPrinted = true;
		}

		/* next node */
		if (reverseOrder)
			temp = temp->prev;
		else
			temp = temp->next;

		/* final newline only if files were allowed display */
		if (!temp && filesPrinted)
			printf("\n");
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
	int dir_list_len;

	for (dir_list_len = 0; temp; dir_list_len++)
		temp = temp->next;
	temp = dir_list_head;

	if (nonFlagArgs < 2)
		onlyOneDir = true;
	else
		if (nonFlagArgs > dir_list_len)
			firstDir = false;
/*
	cocktail_sort_list(&dir_list_head);
*/
	/* if reverse flag is on, advance temp to tail for reversed traversal */
	if (reverseOrder)
		while (temp->next)
			temp = temp->next;

	while (temp)
	{
		if (dirParseAllowed(temp->f_name, cmdLineArgs)
		    && temp->dir_files)
		{
			if ((!onlyOneDir && !firstDir) ||
			    !cmdLineArgs)
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
