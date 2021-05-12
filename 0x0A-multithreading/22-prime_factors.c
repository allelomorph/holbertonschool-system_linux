#include "multithreading.h"


/**
 * create_task - TBD
 *
 * @entry: pointer to the entry function of the task
 * @param: parameter that will later be passed to the entry function
 * Return: ?? TBD
 */
task_t *create_task(task_entry_t entry, void *param)
{
}


/**
 * destroy_task - TBD
 *
 * @task: pointer to the task to destroy
 */
void destroy_task(task_t *task)
{
}


/**
 * exec_tasks - TBD
 *
 * "This function serves as a thread entry; This function can safely return
 *  NULL as its return value will not be retrieved; This function must go
 *  through the list of tasks and execute them, but theres a challenge:
 *  Multiple thread will be going through the list of tasks, and a task must
 *  only be executed once; You must use tprintf to print when a task is
 *  started, and completed"
 *
 * @tasks: pointer to the list of tasks to be executed
 * Return: ?? TBD (list_t defined in list.h)
 */
void *exec_tasks(list_t const *tasks)
{
}
