#include <stdio.h>
#include "test.h"
#include "buffer_user.c"

// Only read/write to by producer
// Input values for char blocks
int CURR_VAL = 0;

void* produce(void* arg) {
	// Generates 1024 byte block of data
	int i;
	char* block = (char*)malloc(sizeof(char*) * DATA_LENGTH);
	memset(block, CURR_VAL, DATA_LENGTH);
	printf("\nBLOCK: [%c]\n", block);
	CURR_VAL++;
}

void* consume(void* arg) {

}

int main(void) {
	//pthread_t t1, t2;
	//void* thread_result;
	//pthread_create(&t1, NULL, produce, 0);
	//pthread_create(&t2, NULL, consume, 0);
	//pthread_join(t1, NULL);
	//pthread_join(t2, NULL);
	void* something;
	produce(something);
	print_semaphores();
	return 0;
}