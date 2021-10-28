#include <stdlib.h>
#include <stdio.h>
#include "buffer.h"

#define HALF_SIZE_BUFFER 10
struct ring_buffer_421 BUFF;

/* Initializes a circular buffer with nodes
*  that hold an int. Tail node points to head
*/
long init_buffer_421() {
	int i;
	struct node_421* curr;
	struct node_421* prev;

	// Fails if buffer already initialized
	if (BUFF.read != NULL) {
		perror("ERROR: Buffer already initialized, cannot initialize");
		//return -1;
	}
	else {
		for (i = 0; i < SIZE_OF_BUFFER; i++) {
			// Create new node for each index
			struct node_421* n_node = (struct node_421*)malloc(sizeof(node_421_t));
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
	}

	//return 0;
}

/* Inserts integer into buffer, increases length
*  and position of write pointer.
*  Fails if buffer is full or uninitialized.
*
* @param[in] i int to write into next node
*/
long insert_buffer_421(int i) {
	// Fails if buffer is full or uninitialized
	if (BUFF.length == SIZE_OF_BUFFER || BUFF.read == NULL) {
		perror("ERROR: Buffer is full or uninitialized, cannot insert");
		//return -1;
	}
	else {
		// Insert data, increment write pointer and length
		BUFF.write->data = i;
		BUFF.write = BUFF.write->next;
		BUFF.length++;
	}

	//return 0;
}

/* Prints all nodes in the buffer.
*  Fails if buffer is uninitialized.
*/
long print_buffer_421() {
	int i;
	struct node_421* curr;

	curr = BUFF.read;

	// Fails if buffer uninitialized
	if (BUFF.read == NULL) {
		perror("ERROR: Buffer uninitialized, cannot print");
		//return -1;
	}
	else {
		i = 1;
		// Prints buffer
		printf(" %d. [%d]\n", i, curr->data);
		curr = curr->next;
		while (curr != BUFF.read) {
			printf(" %d. [%d]\n", i, curr->data);
			curr = curr->next;
			i++;
		}
		printf("\n\n");
	}
	perror("ERROR: Buffer uninitialized, cannot print");
	//return 0;
}

/* Deallocates all memory used in circular buffer.
*  Fails if buffer is uninitialized.
*/
long delete_buffer_421() {
	// Fails if buffer uninitialized
	if (BUFF.read == NULL) {
		perror("ERROR: Buffer is uninitialized, cannot delete");
		//return -1;
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
		BUFF.length = 0;

		//return 0;
	}
}

int main() {
	// Test insert, print, and delete on uninitialized buffer
	insert_buffer_421(1);
	print_buffer_421();
	delete_buffer_421();

	// Initialize buffer, test initializing again
	init_buffer_421();
	init_buffer_421();
	// Fills buffer with values
	int i;
	for (i = 0; i < SIZE_OF_BUFFER; i++) {
		insert_buffer_421(i);
	}

	// Test inserting into full buffer
	insert_buffer_421(1);
	// Print full buffer
	print_buffer_421();
	// Delete buffer
	delete_buffer_421();

	// Test insert, print, and delete on deleted buffer
	insert_buffer_421(1);
	print_buffer_421();
	delete_buffer_421();
	return 0;
}