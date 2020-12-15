#ifndef LAPS_H
#define LAPS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * struct racecar_s - singly linked list node
 * @id: dirver id#
 * @laps: completed lap count
 * @new_entry: was this entry added on this function call?
 * @next: pointer to next node
 */
typedef struct racecar_s
{
	int id;
	int laps;
	bool new_entry;
	struct racecar_s *next;
} racecar_t;

racecar_t *car_list_add_node(int id);
void print_car_list(racecar_t *cars);
void free_car_list(racecar_t **cars);
void race_state(int *id, size_t size);

#endif /* LAPS_H */
