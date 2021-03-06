#include "hls.h"
#include "flags.h"


/**
 * criteriaSort - compares struct member values based on option flags,
 * for sorting of list nodes
 *
 * @node1: node in doubly linked list of structs to be sorted
 * @node2: node in doubly linked list of structs to be sorted
 * Return: difference in compared values
 */
int criteriaSort(file_list_t *node1, file_list_t *node2)
{
	int diff = 0;
	unsigned long nsec1, nsec2;

	/* currently no protections against NULL structs or members */
	/* if ((!node1 || !node2) || (!node1->f_stat || !node2->f_stat)) */

	if (modTimeSort)
	{
		/* newest file first */
		/* check down to 00:00:01 resolution */
		diff = (node2->f_stat->st_mtime - node1->f_stat->st_mtime);
		/* check down to 00:00:00.000000001 resolution */
		if (diff == 0)
		{
			/* AKA st_mtime_nsec, st_mtimensec */
			nsec1 = (unsigned long)node1->f_stat->st_mtim.tv_nsec;
			nsec2 = (unsigned long)node2->f_stat->st_mtim.tv_nsec;
			if (nsec2 > nsec1)
			        return (1);
			else if (nsec2 < nsec1)
				return (-1);
		}
	}

	if (fileSizeSort)
		/* largest size file first; -tS defaults to -S */
		diff = (node2->f_stat->st_size - node1->f_stat->st_size);

	if (diff == 0)
		diff = _strcoll(node1->f_ckey, node2->f_ckey);

	return (diff);
}


/**
 * insertion_sort_list - sorts a doubly linked list of structs in ascending
 *
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
		while (follow && criteriaSort(lead, follow) < 0)
		{ /* lead->crit < follow->crit */
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
 * dll_adj_swap - swaps two adjacent nodes of a doubly linked list
 *
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
 *
 * order using an cocktail shaker sort algorithm
 * @list: doubly linked list of integers to be sorted
 */
void cocktail_sort_list(file_list_t **list)
{
	bool swapped_f, swapped_b;
	int shake_range = 1000000, checks;
	file_list_t *temp;

	if (!list || !(*list) || !(*list)->next)
		return;

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


/**
 * sortDirs - recursively sorts contents of directories; since sorting can
 * change value of head, need to make pass at current level before recursing
 *
 * @file_list: potentially mixed list of files and directories
 */
void sortDirs(file_list_t *file_list)
{
	file_list_t *temp = NULL;

	if (!file_list)
		return;

	temp = file_list;
	while (temp)
	{
		if (temp->dir_files)
			cocktail_sort_list(&(temp->dir_files));
		temp = temp->next;
	}

	temp = file_list;
	while (temp)
	{
		if (temp->dir_files)
			sortDirs(temp->dir_files);
		temp = temp->next;
	}
}
