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

/**
 * criteriaSort - compares struct member values based on option flags,
 * for sorting of list nodes
 * @node1: node in doubly linked list of structs to be sorted
 * @node1: node in doubly linked list of structs to be sorted
 * Return: difference in compared values
 */
int criteriaSort(file_list_t *node1, file_list_t *node2)
{
	int diff = 0;

	/*
	if ((!node1 || !node2) || (!node1->f_stat || !node2->f_stat))
	currently no protections against NULL structs or members
	*/

        if (modTimeSort)
		/* newest file first */
		diff = (node2->f_stat->st_mtime - node1->f_stat->st_mtime);
	if (fileSizeSort)
		/* largest size file first; -tS defaults to -S */
		diff = (node2->f_stat->st_size - node1->f_stat->st_size);
        if (diff == 0)
		diff = aAbBcCmp(node1->f_name, node2->f_name);
/*
	printf("\tcriteriaSort: diff: %i\n", diff);
*/
	return (diff);
}

/**
 * insertion_sort_list - sorts a doubly linked list of structs in ascending
 * order using an insertion sort algorithm
 * @list: doubly linked list of structs to be sorted
 */
void insertion_sort_list(file_list_t **list)
{
        file_list_t *lead, *follow, *new, *temp;

        if (!list || !(*list) || !((*list)->next))
                return;

        /* dance begins with 1st from house left following */
        follow = *list;
        /* and next dancer to house right leading */
        lead = (*list)->next;
        while (lead)
        {
                new = lead->next;
                while (follow && criteriaSort(lead, follow) < 0) /* lead->crit < follow->crit */
                {
                        /* lead and follow swap positions */
                        if (follow->prev)
                                follow->prev->next = lead;
                        else
                                /* if lead makes it to house left, now head */
                                *list = lead;
                        if (lead->next)
                                lead->next->prev = follow;
                        temp = lead->next;
                        lead->next = follow;
                        lead->prev = follow->prev;
                        follow->next = temp;
                        follow->prev = lead;

			/* compare next pair, flowing to house left */
                        follow = lead->prev;
                }
                /* lead sorted to left, new cycle starts @ right leading edge */
                lead = new;
                if (lead)
                        follow = lead->prev;
        }
}


/**
 * _strcmp - compares two strings by ASCII values (adds handling of NULL args,
 * which strcmp lacks)
 *
 * @s1: pointer to the first string
 * @s2: pointer to the second string
 * Return: the difference between the ASCII values of the first non-matching
 * characters, 0 if they match
 */
int _strcmp(const char *s1, const char *s2)
{
	if (s1 == NULL && s2 == NULL)
		return (0);
	else if (s1 == NULL)
		return (0 - s2[0]);
	else if (s2 == NULL)
		return (s1[0]);

	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
        }

	return (*s1 - *s2);
}


int _tolower(char c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + ' ');
	else
		return (c);
}


int _strcasecmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && _tolower(*s1) == _tolower(*s2))
	{
		s1++;
		s2++;
        }

	return (_tolower(*s1) - _tolower(*s2));
}


int aAbBcCmp(const char *a, const char *b)
{
	int diff = _strcasecmp(a, b);

	if (diff)
		return (diff);
	/*
	 * If equal ignoring case, use opposite of strcmp() result to get
	 * lower before upper
	 */
	return (-_strcmp(a, b)); /* or: return strcmp(b, a); */
}

/**
 * dll_adj_swap - swaps two adjacent nodes of a doubly linked list
 * @list: doubly linked list of integers to be sorted
 * @left: node closer to head, right->prev
 * @right: node closer to tail, left->next
 */
void dll_adj_swap(file_list_t **list, file_list_t *left, file_list_t *right)
{
        file_list_t *swap;

        if (left->prev)
                left->prev->next = right;
        else
                *list = right;
        if (right->next)
                right->next->prev = left;
        right->prev = left->prev;
        left->prev = right;
        swap = right;
        left->next = right->next;
        swap->next = left;
}

/**
 * cocktail_sort_list - sorts a doubly linked list of integers in ascending
 * order using an cocktail shaker sort algorithm
 * @list: doubly linked list of integers to be sorted
 */
void cocktail_sort_list(file_list_t **list)
{
        bool swapped_f, swapped_b;
        int shake_range = 1000000, checks;
        file_list_t *temp;

        if (!list || !(*list) || !(*list)->next)
	{
/*
		printf("\tcocktail_sort_list: early exit:"\
		       "list:%p *list:%p (*list)->next:%p\n",
		       (void *)list, list ? (void *)*list : (void *)1, (list && *list) ? (void *)((*list)->next) : (void *)1);
*/
                return;
	}
/*
	printf("\tcocktail_sort_list: normal entry\n");
*/
        temp = *list;
        do {
                swapped_f = swapped_b = false;
                for (checks = 0; temp->next && checks < shake_range; checks++)
                {
                        if (criteriaSort(temp, temp->next) > 0) /* temp->next->n < temp->n */
                        {
                                dll_adj_swap(list, temp, temp->next);
                                swapped_f = true;
                        }
                        else
                                temp = temp->next;
                }
                if (!temp->next)  /* first loop, measuring list */
                        shake_range = checks;
                if (swapped_f)
                        temp = temp->prev;
                shake_range--;
                for (checks = 0; temp->prev && checks < shake_range; checks++)
                {
                        if (criteriaSort(temp, temp->prev) < 0) /* temp->n < temp->prev->n */
                        {
                                dll_adj_swap(list, temp->prev, temp);
                                swapped_b = true;
                        }
                        else
                                temp = temp->prev;
                }
                if (swapped_b)
                        temp = temp->next;
        } while (swapped_f || swapped_b);
}
