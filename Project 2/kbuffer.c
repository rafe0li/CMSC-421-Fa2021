#include <linux/syscalls.h>
#include <linux/kernel.h>
#include "buffer.h"

struct ring_buffer_421 BUFF;

/* Initializes a circular buffer with nodes
*  that hold an int. Tail node points to head
*/
SYSCALL_DEFINE0(init_buffer_421) {
	int i;
	struct node_421* curr;
	struct node_421* prev;

	// Fails if buffer already initialized
	if (BUFF.read != NULL) {
		return -1;
	}

	for (i = 0; i < SIZE_OF_BUFFER; i++) {
		// Create new node for each index
		struct node_421* n_node = (struct node_421*)kmalloc(sizeof(node_421_t), GFP_KERNEL);
		n_node->data = 0;
		n_node->next = NULL;
		curr = n_node;

		// First insertion
		// Initialize read/write pointers
		if (i == 0) {
			BUFF.read = curr;
			BUFF.write = curr;
			prev = curr;
		}

		// Last insertion
		// Set node to point to head, creates circle
		else if (i + 1 == SIZE_OF_BUFFER) {
			prev->next = curr;
			curr->next = BUFF.read;
		}

		// Every other insertion
		// Link previous node to the current node
		else {
			prev->next = curr;
			prev = curr;
		}
	}

	return 0;
}

/* Inserts integer into buffer, increases length
*  and position of write pointer.
*  Fails if buffer is full or uninitialized.
*
* @param[in] i int to write into next node
*/
SYSCALL_DEFINE1(insert_buffer_421, int, i) {
	// Fails if buffer is full or uninitialized
	if (BUFF.length == SIZE_OF_BUFFER || BUFF.read == NULL) {
		return -1;
	}

	// Insert data, increment write pointer and length
	BUFF.write->data = i;
	BUFF.write = BUFF.write->next;
	BUFF.length++;

	return 0;
}

/* Prints all nodes in the buffer.
*  Fails if buffer is uninitialized.
*/
SYSCALL_DEFINE0(print_buffer_421) {
	int i;
	struct node_421* curr;

	curr = BUFF.read;

	// Fails if buffer already initialized
	if (BUFF.read == NULL) {
		return -1;
	}

	// Prints buffer
	for (i = 0; i < SIZE_OF_BUFFER; i++) {
		printk(" %d. [%d]\n", i + 1, curr->data);
		curr = curr->next;
	}
	printk("\n\n");

	return 0;
}

/* Deallocates all memory used in circular buffer.
*  Fails if buffer is uninitialized.
*/
SYSCALL_DEFINE0(delete_buffer_421) {
	// Fails if buffer uninitialized
	if (BUFF.read == NULL) {
		return -1;
	}
	else {
		int i;
		struct node_421* curr;
		struct node_421* prev;

		curr = BUFF.read;
		for (i = 0; i < SIZE_OF_BUFFER; i++) {
			prev = curr;
			curr = curr->next;
			if (prev) {
				free(prev);
			}
		}
		BUFF.read = NULL;
		BUFF.write = NULL;

		return 0;
	}
}