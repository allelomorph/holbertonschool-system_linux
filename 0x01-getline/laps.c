#include "laps.h"

/**
 * car_list_add_node - allocates new node for linked list of racecar ids
 * @id: new id to profile
 * Return: new node in list
 */
racecar_t *car_list_add_node(int id)
{
	racecar_t *new = NULL;

	new = malloc(sizeof(racecar_t));
	if (!new)
	{
		fprintf(stderr, "race_state: Can't malloc!\n");
		return (NULL);
	}
	new->id = id;
	new->laps = 0;
	new->new_entry = true;

	return (new);
}

/**
 * print_car_list - prints linked list of racecar ids and lap counts
 * @cars: list of car profiles
 */
void print_car_list(racecar_t *cars)
{
	racecar_t *temp = NULL;

	temp = cars;
	while (temp)
	{
		if (temp->new_entry)
			printf("Car %i joined the race\n", temp->id);
		temp = temp->next;
	}

	printf("Race state:\n");
	temp = cars;
	while (temp)
	{
		printf("Car %i [%i laps]\n", temp->id, temp->laps);
		temp->new_entry = false;
		temp = temp->next;
	}
}

/**
 * free_car_list - frees linked list of racecar ids and lap counts
 * @cars: list of car profiles
 */
void free_car_list(racecar_t **cars)
{
	racecar_t *head = NULL, *temp = NULL;

	head = *cars;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp);
	}

	*cars = NULL;
}

/**
 * race_state - Tracks lap postion of race cars, updating with every successive
 * call. Every value in either adds a lap to that car's total or adds a new car
 * to the list of competitors
 * @id: array of car ids
 * @size: amount of ids in array `id`
 */
void race_state(int *id, size_t size)
{
	static racecar_t *cars;
	racecar_t *new = NULL, *temp = NULL;
	size_t i;

	if (size == 0)
		free_car_list(&cars);

	if (size == 0 || id == NULL)
		return;

	for (i = 0; i < size; i++)
	{
		temp = cars;
		while (temp && temp->next && temp->next->id <= id[i])
			temp = temp->next; /* find insertion point */

		if (temp && temp->id == id[i])
		{
			temp->laps++;
			continue;
		}

		new = car_list_add_node(id[i]);
		if (!new)
			return;

		/* insert at head */
		if (!temp || (temp == cars && temp->id > id[i]))
		{
			new->next = cars;
			cars = new;
		}
		else /* insert before next higher value, or at tail if none */
		{
			new->next = temp->next;
			temp->next = new;
		}
	}

	print_car_list(cars);
}
