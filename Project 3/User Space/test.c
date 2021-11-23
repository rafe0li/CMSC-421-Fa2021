/*
* @author Rafael Li, rafaell1@umbc.edu
* @test.c
* Process which runs two threads, a producer and
* a consumer. The producer thread enqueues 1024 byte long
* char* into a circular buffer, and the consumer dequeues
* these blocks, prints them out, and deallocates them.
* Threads run concurrently and are protected by semaphores
* - conditions include mutual exclusion for access, a full buffer,
* and an empty buffer. If full, producer blocks and waits for consumer,
* vice versa.
*
*/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "test.h"
#include "buffer_user.c"

// Only modified by producer
// Input values for data blocks
int CURR_VAL = 0;

/**
 * Produce a 1024 byte char* to enqueue into circular buffer.
 * @param[in] arg void*, for threading
 */
void* produce(void* arg) {
	while (1) {
		printf("\nENTERED PRODUCER\n");
		unsigned int stop;

		// Create 1024 byte char block
		char val = CURR_VAL + '0';
		char* block = (char*)malloc(sizeof(char) * DATA_LENGTH);
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
		print_buffer_421();
		printf("\nLEFT PRODUCER\n");
	}
}

/**
 * Dequeue the next value in the buffer and consume it (print it out).
* @param[in] arg void*, for threading
 */
void* consume(void* arg) {
	while (1) {
		printf("\nENTERED CONSUMER\n");
		unsigned int stop;
		char* block = (char*)malloc(sizeof(char) * DATA_LENGTH);

		// Wait 0-100 milliseconds before each operation
		stop = rand() % 10000;
		usleep(stop);

		// Dequeues and consumes data block
		dequeue_buffer_421(block);
		printf("\nBLOCK: [%.*s]\n", DATA_LENGTH, block);
		free(block);
		print_buffer_421();
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