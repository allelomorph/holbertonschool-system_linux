#ifndef _BINARY_TREES_H_
#define _BINARY_TREES_H_

#include <stddef.h>

/**
 * struct binary_tree_s - Binary tree node
 *
 * @n: Integer stored in the node
 * @parent: Pointer to the parent node
 * @left: Pointer to the left child node
 * @right: Pointer to the right child node
 */
typedef struct binary_tree_s
{
	int n;
	struct binary_tree_s *parent;
	struct binary_tree_s *left;
	struct binary_tree_s *right;
} binary_tree_t;

void binary_tree_print(const binary_tree_t *);

/* begin student addtions to binary_trees.h */
typedef struct binary_tree_s heap_t;

/* task 0 */
binary_tree_t *binary_tree_node(binary_tree_t *parent, int value);

/* task 1 helpers */
size_t max_size_t(size_t a, size_t b);
size_t binary_tree_height(const binary_tree_t *tree);
size_t binary_tree_nodes(const binary_tree_t *tree);
int binary_tree_is_perfect(const binary_tree_t *tree);

/* task 1 novel functions */
heap_t *binary_tree_complete_insert(heap_t *tree, int value);
heap_t *heap_insert(heap_t **root, int value);

#endif /* _BINARY_TREES_H_ */
