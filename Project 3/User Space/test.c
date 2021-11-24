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

/*
 * Produce a 1024 byte char* to enqueue into circular buffer.
 * @param[in] arg void*, for threading
 */
void* produce(void* arg) {
	int i;
	for (i = 0; i < 100000; i++) {
		unsigned int stop;
		// Create temp 1024 byte char block to enqueue
		char val = CURR_VAL + '0';
		char* block = (char*)malloc(sizeof(char) * DATA_LENGTH);
		memset(block, val, DATA_LENGTH);

		// Wait 0-100 milliseconds before each operation
		stop = rand() % 10000;
		usleep(stop);

		// Enqueue, increment input value, and prints buffer
		printf("\n\n:: Enqueueing element into buffer. ::");
		printf("\nPRODUCED BLOCK = [%.10s...]", block);
		enqueue_buffer_421(block);
		CURR_VAL++;
		if (CURR_VAL == 10) {
			CURR_VAL = 0;
		}
		print_buffer_421();
		free(block);
	}
}

/*
 * Dequeue the next value in the buffer and consume it (print it out).
 * @param[in] arg void*, for threading
 */
void* consume(void* arg) {
	int i;
	for (i = 0; i < 100000; i++) {
		unsigned int stop;
		char* block = (char*)malloc(sizeof(char) * DATA_LENGTH);

		// Wait 0-100 milliseconds before each operation
		stop = rand() % 10000;
		usleep(stop);

		// Dequeues, consumes data block, and prints buffer
		printf("\n\n:: Dequeueing element from buffer. ::");
		dequeue_buffer_421(block);
		printf("\nCONSUMED BLOCK = [%.10s...]\n", block);
		print_buffer_421();
		free(block);
	}
}

/*
 * Runs two separate producer/consumer threads.
*/
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