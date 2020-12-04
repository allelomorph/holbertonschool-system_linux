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

char *modeString(mode_t mode)
{
	char mode_s[11];

	switch (mode & S_IFMT) {
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

	return _strcopy(mode_s);
}

char *dateTimeString(time_t time)
{
	char *time_s, *fmt_time_s;
	int i;

	fmt_time_s = malloc(sizeof(char) * 13);

	/* Fri Dec  4 02:49:57 2020 - 26 chars including \n and \0 ? */
	time_s = ctime(&time);

	for (i = 4; i < 16; i++)
		fmt_time_s[i - 4] = time_s[i];
	fmt_time_s[12] = '\0';

	/* Dec  4 02:49 - 13 chars incl. \0, no \n */
	return fmt_time_s;
}

int longFormatPrint(file_list_t *node)
{
	char *mode_buf, *time_buf;
	struct passwd *usr;
	struct group *grp;

	if (!node || !node->f_stat)
	{
		fprintf(stderr, "longFormatPrint: missing f_stat");
		return EXIT_FAILURE;
	}

        mode_buf = modeString(node->f_stat->st_mode);
	time_buf = dateTimeString(node->f_stat->st_mtime);
	usr = getpwuid(node->f_stat->st_uid);
	grp = getgrgid(node->f_stat->st_gid);
	printf("%s %u %s %s %4u %s %s",
	       mode_buf,
	       (unsigned)node->f_stat->st_nlink,
	       usr->pw_name ? usr->pw_name : "",
	       grp->gr_name ? grp->gr_name : "",
	       (unsigned)node->f_stat->st_size,
	       time_buf,
	       node->f_name);

	if (node->f_slnk)
		printf(" -> %s", node->f_slnk);

	free(mode_buf);
	free(time_buf);
	return EXIT_SUCCESS;
}

void accessError(const char *file)
{
	fprintf(stderr, "hls: cannot access ");
	perror(file);
}


bool stringExactMatch(char *s1, char *s2)
{
        while (s1 && s2)
	{
		if (*s1 != *s2)
			return false;
		else if (*s1 == '\0' && *s2 == '\0')
		        return true;
		s1++;
		s2++;
	}
	return false;
}
