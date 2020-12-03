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
/* consider making a `defaultDir` flag for when no args or only flag args */

int main(int argc, char *argv[])
{
	file_list_t *file_list = NULL;
	file_list_t *dir_list = NULL;

	/* need to bring in _strcmp() from foundations!! */
	/* and what sorting algo? insertion?? */
	parseArgs(argc, argv, &file_list, &dir_list);

	/* sortList(flie or dir list)
	     sort lists based on flags (default alpha lowercase first)
	     -l -S and -t will require a list of stat structs not strings
	*/

	/* listFiles(file list/array from args)
	      prints file args (maybe sorted) in single line before dirs
	*/

	/* loop through -- listDirectory(file list/array from args)
	      !! if both file list and dir list empty, then listDir `.`
	      prints dirs in dir list first by <name>:\n, and then as a file list
	      this is where -a and -A will filter output
	 */

	testPrintFlags();

	printf("files:\n");
	testPrintList(file_list);
	printf("dirs:\n");
	testPrintList(dir_list);

	/* cleanup/free func on lists and buffers */

	return EXIT_SUCCESS;
}
