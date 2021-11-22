#include <stdio.h>
#include "test.h"
#include "buffer_user.c"

// Only read/write to by producer
// Input values for char blocks
int CURR_VAL = 0;

void* produce(void* arg) {
	// Generates 1024 byte block of data
	int i;
	// Convert int to equivalent (0-9) char
	char val = CURR_VAL + '0';
	char* block = (char*)malloc(sizeof(char*) * DATA_LENGTH);
	memset(block, val, (sizeof(char*) * DATA_LENGTH));
	printf("\nBLOCK: [%s]\n", block);
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