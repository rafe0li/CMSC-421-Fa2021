#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "test.h"
#include "buffer_user.c"

// Only modified by producer
// Input values for data blocks
int CURR_VAL = 0;

void* produce(void* arg) {
	int i;
	for (i = 0; i < 2; i++) {
		printf("\nENTERED PRODUCER\n");
		unsigned int stop;

		// Create 1024 byte char block
		char val = CURR_VAL + '0';
		char* block = (char*)malloc(sizeof(char*) * DATA_LENGTH);
		memset(block, val, DATA_LENGTH);

		// Wait 0-100 milliseconds before each operation
		stop = rand() % 10000;
		usleep(stop);

		// Enqueue and increment input value
		enqueue_buffer_421(block);
		CURR_VAL++;
		if (CURR_VAL == 10) {
			CURR_VAL = 0;
		}
		free(block);

		printf("\nBUFFER: ");
		print_queue();
		printf("\n");
		printf("\nLEFT PRODUCER\n");
	}
}

void* consume(void* arg) {
	int i;
	for (i = 0; i < 2; i++) {
		printf("\nENTERED CONSUMER\n");
		unsigned int stop;
		char* block = (char*)malloc(sizeof(char*) * DATA_LENGTH);

		// Wait 0-100 milliseconds before each operation
		stop = rand() % 10000;
		usleep(stop);

		// Dequeues and consumes data block
		dequeue_buffer_421(block);
		printf("\nCONSUMED BLOCK: [%s]\n", block);
		free(block);

		printf("\nBUFFER: ");
		print_queue();
		printf("\n");
		printf("\nLEFT CONSUMER\n");
	}
}

int main(void) {
	init_buffer_421();
	pthread_t t1, t2;
	pthread_create(&t1, NULL, produce, NULL);
	sleep(4);
	pthread_create(&t2, NULL, consume, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	delete_buffer_421();
	return 0;
}