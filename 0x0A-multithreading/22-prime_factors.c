#include "multithreading.h"
/* malloc free */
#include <stdlib.h>

static pthread_mutex_t task_status_mutex;

/**
 * initTaskStatusMutex - initializes global mutex changing task status before
 *   main
 */
void initTaskStatusMutex(void)
{
	pthread_mutex_init(&task_status_mutex, NULL);
}


/**
 * destroyTaskStatusMutex - destroys global mutex for changing task status
 *   after main
 */
void destroyTaskStatusMutex(void)
{
	pthread_mutex_destroy(&task_status_mutex);
}


/**
 * create_task - allocates and initializes a task_entry_t task profile for use
 *   in pthread multithreading
 *
 * @entry: pointer to the entry function of the task
 * @param: parameter that will later be passed to the entry function
 * Return: pointer to newly created task profile, or NULL on failure
 */
task_t *create_task(task_entry_t entry, void *param)
{
	task_t *task = NULL;

	if (!entry)
		return (NULL);

	task = malloc(sizeof(task_t));
	if (!task)
		return (NULL);

	task->entry = entry;
	task->param = param; /* expect `list_t *` in this exercise */
	task->status = PENDING;
	task->result = NULL;
	/* mutex is initialized in constructor initTaskStatusMutex */
	task->lock = task_status_mutex;

	return (task);
}


/**
 * destroy_task - deallocatesa task_entry_t task profile, as used in pthread
 *   multithreading
 *
 * @task: pointer to the task to destroy
 */
void destroy_task(task_t *task)
{
	if (!task)
		return;

	/*
	 * In this project, task->param can be expected to be a string from
	 * argv, which does not need to be freed. There is currently no way of
	 * knowing from outside this function what the data type and thus
	 * freeing function for task->param is.
	 */

	/*
	 * In this project, task->result can be expected to be `list_t *` with
	 * data of `unsigned long *`. Similar to task->param, there is
	 * currently no way of knowing from outside this function what the data
	 * type and thus freeing function for task->result is.
	 */
	if (task->result)
	{
		list_destroy(task->result, free);
		free(task->result);
	}

	/* task->lock destroyed in destructor destroyTaskStatusMutex */

	free(task);
}


/**
 * exec_tasks - entry function to a thread which starts any tasks in a task
 *   list that have not been started by other threads
 *
 * Note: In project description: "This function can safely return
 *  NULL as its return value will not be retrieved." Perhaps prototype cannot
 *  just return `void` since pthread_create() requires a function pointer of
 *  type void *(*) (void *) ?
 *
 * @tasks: pointer to the list of tasks to be executed
 * Return: always returns NULL (see note above)
 */
void *exec_tasks(list_t const *tasks)
{
	node_t *curr_node = NULL;
	task_t *task = NULL;
	size_t i;

	if (!tasks || !tasks->head)
		return (NULL);

	for (i = 0, curr_node = tasks->head; i < tasks->size;
	     i++, curr_node = curr_node->next)
	{
		task = (task_t *)curr_node->content;
		if (!task)
			continue;

		pthread_mutex_lock(&task_status_mutex);
		if (task->status == PENDING)
		{
			task->status = STARTED;
			pthread_mutex_unlock(&task_status_mutex);

			tprintf("[%02lu] Started\n", i);
			task->result = task->entry(task->param);
			if (!task->result)
			{
				task->status = FAILURE;
				tprintf("[%02lu] Failure\n", i);
			}
			else
			{
				task->status = SUCCESS;
				tprintf("[%02lu] Success\n", i);
			}
		}
		else
			pthread_mutex_unlock(&task_status_mutex);
	}

	return (NULL);
}
