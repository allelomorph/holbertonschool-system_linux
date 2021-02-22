#include "hls.h"
#include "flags.h"

/**
 * modeString - formats a string to represent a st_mode value for a file
 * @mode: mode_t value for bitmasking from a stat struct
 * Return: formatted string with file type and permissions
 */
char *modeString(mode_t mode)
{
	char mode_s[11];

	switch (mode & S_IFMT)
	{
	case S_IFREG:
		mode_s[0] = '-';
		break;
	case S_IFDIR:
		mode_s[0] = 'd';
		break;
	case S_IFBLK:
		mode_s[0] = 'b';
		break;
	case S_IFCHR:
		mode_s[0] = 'c';
		break;
	case S_IFSOCK:
		mode_s[0] = 's';
		break;
	case S_IFLNK:
		mode_s[0] = 'l';
		break;
	case S_IFIFO:
		mode_s[0] = 'p';
		break;
	}
	mode_s[1] = (mode & S_IRUSR) ? 'r' : '-';
	mode_s[2] = (mode & S_IWUSR) ? 'w' : '-';
	mode_s[3] = (mode & S_IXUSR) ? 'x' : '-';
	mode_s[4] = (mode & S_IRGRP) ? 'r' : '-';
	mode_s[5] = (mode & S_IWGRP) ? 'w' : '-';
	mode_s[6] = (mode & S_IXGRP) ? 'x' : '-';
	mode_s[7] = (mode & S_IROTH) ? 'r' : '-';
	mode_s[8] = (mode & S_IWOTH) ? 'w' : '-';
	mode_s[9] = (mode & S_IXOTH) ? 'x' : '-';
	mode_s[10] = '\0';

	return (_strcopy(mode_s));
}

/**
 * dateTimeString - selectively edits the output of `ctime`
 * @time: time_t time value from a stat struct
 * Return: formatted string with shortened time format
 */
char *dateTimeString(time_t time)
{
	char *time_s = NULL, *fmt_time_s = NULL;
	int i;

	fmt_time_s = malloc(sizeof(char) * 13);

	/* Fri Dec  4 02:49:57 2020 - 26 chars including \n and \0 ? */
	time_s = ctime(&time);

	for (i = 4; i < 16; i++)
		fmt_time_s[i - 4] = time_s[i];
	fmt_time_s[12] = '\0';

	/* Dec  4 02:49 - 13 chars incl. \0, no \n */
	return (fmt_time_s);
}

/**
 * longFormatPrint - formats long-form output of file info based on stat struct
 * @node: node in a linked list of file profiles
 * Return: exit macros for success and failure
 */
int longFormatPrint(file_list_t *node)
{
	char *mode_buf = NULL, *time_buf = NULL,
		*usrn_buf = NULL, *grpn_buf = NULL;
	struct passwd *usr = NULL;
	struct group *grp = NULL;

	if (!node || !node->f_stat)
	{
		fprintf(stderr, "longFormatPrint: missing f_stat");
		return (EXIT_FAILURE);
	}

	mode_buf = modeString(node->f_stat->st_mode);
	time_buf = dateTimeString(node->f_stat->st_mtime);
	usr = getpwuid(node->f_stat->st_uid);
	if (usr)
		usrn_buf = usr->pw_name;
	grp = getgrgid(node->f_stat->st_gid);
	if (grp)
		grpn_buf = grp->gr_name;

	/* file type, permissions, and number of hard links */
	printf("%s %2u ",
	       mode_buf,
	       (unsigned int)node->f_stat->st_nlink);

	/* user name or UID if not named */
	if (usrn_buf)
		printf("%s ", usrn_buf);
	else
		printf("%3u ", (unsigned int)node->f_stat->st_uid);

	/* group name or GID if not named */
	if (grpn_buf)
		printf("%s ", grpn_buf);
	else
		printf("%3u ", (unsigned int)node->f_stat->st_gid);

	/* file size in bytes, last modifcation time, name */
	printf("%4u %s %s",
	       (unsigned int)node->f_stat->st_size,
	       time_buf,
	       node->f_name);

	/* symbolic link destination */
	if (node->f_slnk)
		printf(" -> %s", node->f_slnk);

	free(mode_buf);
	free(time_buf);
	return (EXIT_SUCCESS);
}

/**
 * stringExactMatch - goes beyond strcmp to ensure that the two strings match to
 * the null bit
 * @s1: first string to compare
 * @s2: second string to compare
 * Return: boolean representing exact match
 */
bool stringExactMatch(const char *s1, char *s2)
{
	while (s1 && s2)
	{
		if (*s1 != *s2)
			return (false);
		else if (*s1 == '\0' && *s2 == '\0')
			return (true);
		s1++;
		s2++;
	}
	return (false);
}

