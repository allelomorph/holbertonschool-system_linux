#include "hls.h"
#include "flags.h"

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
		diff = _strcoll(node1->f_ckey, node2->f_ckey);
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
