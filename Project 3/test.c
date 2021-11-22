#include <stdio.h>
#include "test.h"
#include "buffer_user.c"

// Only read/write to by producer
// Input values for char blocks
int CURR_VAL = 0;

void* produce(void* arg) {
	// Generates 1024 byte block of data
	int i;
	// Converts int to equivalent (0-9) char
	char val = CURR_VAL + '0';
	char* block = (char*)malloc(sizeof(char*) * DATA_LENGTH);
	memset(block, val, (sizeof(char*) * DATA_LENGTH));
	printf("\nBLOCK: [%s]\n", block);
	CURR_VAL++;
}

void* consume(void* arg) {

}

int main(void) {
	char* result = (char*)malloc(sizeof(char*) * DATA_LENGTH);
	init_buffer_421();
	//pthread_t t1, t2;
	//pthread_create(&t1, NULL, produce, 0);
	//pthread_create(&t2, NULL, consume, 0);
	//pthread_join(t1, NULL);
	//pthread_join(t2, NULL);
	print_semaphores();
	
	enqueue_buffer_421("1");
	print_queue();
	enqueue_buffer_421("2");
	print_queue();
	enqueue_buffer_421("3");
	print_queue();
	enqueue_buffer_421("4");
	print_queue();
	dequeue_buffer_421(result);
	print_queue();
	printf("\nDEQUEUED VALUE: [%s]\n", result);

	if (result) {
		free(result);
	}

	delete_buffer_421();
	return 0;
}