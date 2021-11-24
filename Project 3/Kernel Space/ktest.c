/*
* @author Rafael Li, rafaell1@umbc.edu
* @test.c
* KERNEL VERSION
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
#include <linux/kernel.h>
#include <sys/syscall.h>
#include "test.h"

#define __NR_init_buffer_421 442
#define __NR_enqueue_buffer_421 443
#define __NR_dequeue_buffer_421 444
#define __NR_delete_buffer_421 445
#define __NR_print_buffer_421 446

long sys_init_buffer_421(void) {
	return syscall(__NR_init_buffer_421);
}

long sys_enqueue_buffer_421(char* arg) {
	return syscall(__NR_enqueue_buffer_421, arg);
}

long sys_dequeue_buffer_421(void) {
	return syscall(__NR_dequeue_buffer_421);
}

long sys_delete_buffer_421(void) {
	return syscall(__NR_delete_buffer_421);
}

long sys_print_buffer_421(void) {
	return syscall(__NR_print_buffer_421);
}

// Only modified by producer
// Input values for data blocks
int CURR_VAL = 0;

/*
 * Produce a 1024 byte char* to enqueue into circular buffer.
 * @param[in] arg void*, for threading
 */
void* produce(void* arg) {
	int i;
	long call;
	for (i = 0; i < 100000; i++) {
		unsigned int stop;
		// Create temp 1024 byte char block to enqueue
		char val = CURR_VAL + '0';
		char* block = (char*)malloc(sizeof(char) * DATA_LENGTH);
		memset(block, val, DATA_LENGTH);

		// Wait 0-100 milliseconds before each operation
		stop = rand() % 10000;
		usleep(stop);

		// Enqueue and increment input value
		printf("\n:: Enqueueing element into buffer. ::");
		call = sys_enqueue_buffer_421(1);
		if (call < 0) {
			perror("ERROR WITH ENQUEUEING");
		}
		else {
			printf("enqueue_buffer ran successfully, check dmesg output\n");
		}
		CURR_VAL++;
		if (CURR_VAL == 10) {
			CURR_VAL = 0;
		}
		printf("\n%.10s...", block);

		// Print buffer result and free temp block
		call = sys_print_buffer_421(1);
		if (call < 0) {
			perror("ERROR WITH PRINTING");
		}
		else {
			printf("print_buffer ran successfully, check dmesg output\n");
		}
		free(block);
	}
}

/*
 * Dequeue the next value in the buffer and consume it (print it out).
 * @param[in] arg void*, for threading
 */
void* consume(void* arg) {
	int i;
	long call;
	for (i = 0; i < 100000; i++) {
		unsigned int stop;
		char* block = (char*)malloc(sizeof(char) * DATA_LENGTH);

		// Wait 0-100 milliseconds before each operation
		stop = rand() % 10000;
		usleep(stop);

		// Dequeues and consumes data block
		printf("\n:: Dequeueing element from buffer. ::");
		call = sys_dequeue_buffer_421(1);
		if (call < 0) {
			perror("ERROR WITH DEQUEUEING");
		}
		else {
			printf("dequeue_buffer ran successfully, check dmesg output\n");
		}
		printf("\n%.10s...", block);
		free(block);

		// Print buffer result
		call = sys_print_buffer_421(1);
		if (call < 0) {
			perror("ERROR WITH PRINTING");
		}
		else {
			printf("print_buffer ran successfully, check dmesg output\n");
		}
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