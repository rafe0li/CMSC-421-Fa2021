#include <stdlib.h>
#include <stdio.h>
#include "buffer.h"
struct ring_buffer_421 BUFF;

// Commented return conditions for syscall failing
long init_buffer_421() {
	int i;
	struct node_421* curr = (struct node_421*)malloc(sizeof(node_421_t));
	struct node_421* prev;

	// Buffer already initialized
	//if (BUFF.read != NULL) {
		//return -1;
	//}

	for (i = 0; i < SIZE_OF_BUFFER; i++) {
		curr->data = 0;
		curr->next = NULL;
		if (i == 0) {
			BUFF.read = (struct node_421*)malloc(sizeof(node_421_t));
			BUFF.read = curr;
			BUFF.write = (struct node_421*)malloc(sizeof(node_421_t));
			BUFF.write = curr;
		}
		else {
			prev->next = curr;
		}
		prev = curr;
		// Newly created node
		struct node_421* n_node = (struct node_421*)malloc(sizeof(node_421_t));
		curr = n_node;
	}

	//return 0;
}

long insert_buffer_421(int i) {
	// Buffer full
	//if (BUFF.read == BUFF.write)
		//return -1;
	BUFF.write->data = i;
	BUFF.write = BUFF.write->next;
	BUFF.length++;
	//return 0;
}

long print_buffer_421() {
	int i;
	struct node_421* curr = (struct node_421*)malloc(sizeof(struct node_421));
	curr = BUFF.read;

	for (i = 0; i < SIZE_OF_BUFFER; i++) {
		printf("[%d]\t", curr->data);
		curr = curr->next;
	}
}

long delete_buffer_421() {

}

int main() {
	init_buffer_421();
	print_buffer_421();
	return 0;
}