#include "hls.h"

/* init flags */
bool singleColumn   = false;
bool allFiles       = false;
bool almostAllFiles = false;
bool longFormat     = false;
bool reverseOrder   = false;
bool fileSizeSort   = false;
bool modTimeSort    = false;
bool Recursive      = false;

/* error handling */
int exitCode = EXIT_SUCCESS;

/**
 * main - entry point into `hls`, a clone of tha bash function `ls`
 * @argc: number of command line arguments
 * @argv: array of command line arguments
 * Return: EXIT_SUCCESS, EXIT_FAILURE for minor problems, and 2 for major ones
 */
int main(int argc, char *argv[])
{
	int i, nonFlagArgs = 0;
	file_list_t *file_list = NULL;
	file_list_t *dir_list = NULL;
	file_list_t *temp = NULL;
	bool cmdLineArgs = true;

	/* first pass through argv to set option flags */
	for (i = 0; i < argc; i++)
		if (argv[i][0] == '-')
			setFlags(argv[i]);

	/* second pass through argv to store file/dir profiles for sorting */
	nonFlagArgs = parseArgs(argc, argv, &file_list, &dir_list);

	/* sort lists based on flags (default alpha lowercase first) */
	/* no hidden file screen on files from args, only on files from dirs */

	if (file_list)
	{
		cocktail_sort_list(&file_list);
		printFileList(file_list, cmdLineArgs);

	}

	if (dir_list)
	{
		/* populates dir profiles with file profile list of contents*/
		parseDirs(dir_list, cmdLineArgs);
/*
		testPrintList(dir_list);
*/
		cocktail_sort_list(&dir_list);

		temp = dir_list;
		while (temp)
		{
			cocktail_sort_list(&(temp->dir_files));
/*
		printf("\tbefore cocktail_sort_list:"\
		       "temp->dir_files:%p &(temp->dir_files):%p *(&(temp->dir_files)):%p\n",
		       (void *)temp->dir_files, (void *)&(temp->dir_files), (void *)*(&(temp->dir_files)));
*/
			temp = temp->next;
		}

		printDirs(dir_list, cmdLineArgs, nonFlagArgs);
	}
/*
	printf("-----file_list:\n");
	testPrintList(file_list);
	printf("-----dir_list:\n");
	testPrintList(dir_list);
*/
	/* cleanup lists and buffers */
	if (file_list)
		freeList(file_list);
	if (dir_list)
		freeList(dir_list);

	return (exitCode);
}
