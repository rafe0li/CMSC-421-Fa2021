#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "test.h"
#include "buffer_user.c"

// Only modified by producer
// Input values for data blocks
int CURR_VAL = 0;

void* produce(void* arg) {
	printf("\nENTERED PRODUCER THREAD\n");
	int stop;

	// Create 1024 byte char block
	char val = CURR_VAL + '0';
	char* block = (char*)malloc(sizeof(char*) * DATA_LENGTH);
	memset(block, val, DATA_LENGTH);

	// Wait 0-100 milliseconds before each operation
	stop = rand() % 10000;
	usleep(stop);

	printf("\nBEFORE ENQUEUE\n");
	enqueue_buffer_421(block);
	printf("\nAFTER ENQUEUE\n");
	CURR_VAL++;
	if (CURR_VAL == 10) {
		CURR_VAL = 0;
	}

	printf("\nENQUEUE: ");
	print_queue();
	printf("\n");
	printf("\nLEFT PRODUCER THREAD\n");
}

void* consume(void* arg) {
	printf("\nENTERED CONSUMER THREAD\n");
	int stop;
	char* block;

	// Wait 0-100 milliseconds before each operation
	stop = rand() % 10000;
	usleep(stop);

	// Dequeues and consumes
	dequeue_buffer_421(block);
	free(block);

	printf("\nDEQUEUE: ");
	print_queue();
	printf("\n");
	printf("\nLEFT CONSUMER THREAD\n");
}

int main(void) {
	init_buffer_421();
	print_semaphores();
	pthread_t t1, t2;
	pthread_create(&t1, NULL, produce, NULL);
	sleep(4);
	pthread_create(&t2, NULL, consume, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	print_semaphores();
	delete_buffer_421();
	return 0;
}