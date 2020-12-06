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

int main(int argc, char *argv[])
{
	int i;
	file_list_t *file_list = NULL;
	file_list_t *dir_list = NULL;
	bool cmdLineArgs = true, fileArgsEmpty = true;

	/* first pass through argv to set option flags */
	for (i = 0; i < argc; i++)
		if (argv[i][0] == '-')
			setFlags(argv[i]);

	/* second pass through argv to store file/dir profiles for sorting */
	parseArgs(argc, argv, &file_list, &dir_list);

	/* sort lists based on flags (default alpha lowercase first) */
	/* no hidden file screen on files from args, only on files from dirs */

	if (file_list)
	{
		fileArgsEmpty = false;
		printFileList(file_list, cmdLineArgs);
	}

	if (dir_list)
	{
		/* populates dir profiles with file profile list of contents*/
		parseDirs(dir_list, cmdLineArgs);
		printDirs(dir_list, cmdLineArgs, fileArgsEmpty);
	}
	/*
	testPrintList(file_list);
	testPrintList(dir_list);
	*/
	/* cleanup lists and buffers */
	if (file_list)
		freeList(file_list);
	if (dir_list)
		freeList(dir_list);

	return (EXIT_SUCCESS);
}
