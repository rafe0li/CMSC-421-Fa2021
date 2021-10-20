#include "buffer.h"
struct ring_buffer_421 BUFF = malloc(sizeof(struct ring_buffer_421_t));
struct node_421* HEAD = (struct node_421*)malloc(sizeof(struct node_421_t));
struct node_421* TAIL = (struct node_421*)malloc(sizeof(struct node_421_t));


long init_buffer_421() {
	int i;
	struct node_421* curr = (struct node_421*)malloc(sizeof(struct node_421_t));
	struct node_421* prev;
	for (i = 0; i < SIZE_OF_BUFFER; i++) {
		curr.data = 0;
		curr->curr = NULL;
		if (i == 0) {
			HEAD = curr;
		}
		else {
			if (i == SIZE_OF_BUFFER - 1) {
				TAIL = curr;
			}
			prev->curr = curr;
			prev = curr;
		}
		curr = (struct node_421*)realloc(curr, sizeof(struct node_421_t));
	}
}

long insert_buffer_421(int i) {

}

long print_buffer_421() {

}

long delete_buffer_421() {

}

int main() {
	init_buffer_421();
	return 0;
}