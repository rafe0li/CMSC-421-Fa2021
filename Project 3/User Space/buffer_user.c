/*
* @author Rafael Li, rafaell1@umbc.edu
* @buffer_user.c
* Circular buffer of 20 nodes that hold 1024 byte
* char*. Enqueueing and deqeueing are thread safe,
* protected by semaphores. Enqueue will block if the
* buffer is full and call dequeue, and vice versa for
* an empty buffer.
*
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "buffer.h"
#include "buffer_util.h"

static ring_buffer_421_t buffer;
static sem_t mutex;
static sem_t fill_count;
static sem_t empty_count;

long init_buffer_421(void) {
	// Note: You will need to initialize semaphores in this function.
	// Ensure we're not initializing a buffer that already exists.
	if (buffer.read || buffer.write) {
		printf("init_buffer_421(): Buffer already exists. Aborting.\n");
		//return -1;
	}

	// Create the root node.
	node_421_t *node;
	node = (node_421_t *) malloc(sizeof(node_421_t));
	// Create the rest of the nodes, linking them all together.
	node_421_t *curr;
	int i;
	curr = node;
	// Note that we've already created one node, so i = 1.
	for (i = 1; i < SIZE_OF_BUFFER; i++) {
		curr->next = (node_421_t *) malloc(sizeof(node_421_t));
		curr = curr->next;
	}
	// Complete the chain.
	curr->next = node;
	buffer.read = node;
	buffer.write = node;
	buffer.length = 0;

	// Initialize semaphores
	sem_init(&mutex, 0, 1);
	sem_init(&fill_count, 0, 1);
	sem_init(&empty_count, 0, 1);

	//return 0;
}

long enqueue_buffer_421(char * data) {
	if (!buffer.write) {
		printf("enqueue_buffer_421(): The buffer does not exist. Aborting.\n");
		//return -1;
	}

	// If buffer empty, signal enqueue and wait
	if (buffer.length == 20) {
		sem_post(&empty_count);
		sem_wait(&fill_count);
	}

	sem_wait(&mutex);

	memset(buffer.write->data, 0, DATA_LENGTH);
	memcpy(buffer.write->data, data, DATA_LENGTH);
	// Advance the pointer.
	buffer.write = buffer.write->next;
	buffer.length++;

	sem_post(&mutex);

	if (buffer.length == 20) {
		sem_post(&empty_count);
	}

	//return 0;
}

long dequeue_buffer_421(char * data) {
	node_421_t* curr;
	node_421_t* prev;

	// Empty buffer condition
	if (!buffer.write) {
		printf("dequeue_buffer_421(): The buffer does not exist. Aborting.\n");
		//return -1;
	}

	// If buffer empty, signal enqueue and wait
	if (buffer.length == 0) {
		sem_post(&fill_count);
		sem_wait(&empty_count);
	}
	sem_wait(&mutex);

	// Copies data from buffer into "data" param
	memcpy(data, buffer.read->data, DATA_LENGTH);
	buffer.length--;

	// Moves all values up in queue
	curr = buffer.read;
	while (curr->next != buffer.read) {
		prev = curr;
		curr = curr->next;
		memcpy(prev->data, curr->data, DATA_LENGTH);
	}

	sem_post(&mutex);

	if (buffer.length == 0) {
		sem_post(&fill_count);
	}

	//return 0;
}

long delete_buffer_421(void) {
	// Tip: Don't call this while any process is waiting to enqueue or dequeue.
	if (!buffer.read) {
		printf("delete_buffer_421(): The buffer does not exist. Aborting.\n");
		//return -1;
	}
	// Get rid of all existing nodes.
	node_421_t *temp;
	node_421_t *current = buffer.read->next;
	while (current != buffer.read) {
		temp = current->next;
		free(current);
		current = temp;
	}
	// Free the final node.
	free(current);
	current = NULL;
	// Reset the buffer.
	buffer.read = NULL;
	buffer.write = NULL;
	buffer.length = 0;
	//return 0;
}

void print_semaphores(void) {
	// You can call this method to check the status of the semaphores.
	// Don't forget to initialize them first!
	// YOU DO NOT NEED TO IMPLEMENT THIS FOR KERNEL SPACE.
	int value;
	sem_getvalue(&mutex, &value);
	printf("sem_t mutex = %d\n", value);
	sem_getvalue(&fill_count, &value);
	printf("sem_t fill_count = %d\n", value);
	sem_getvalue(&empty_count, &value);
	printf("sem_t empty_count = %d\n", value);
	//return;
}

void print_buffer_421(void) {
	int i = 0;
	node_421_t* curr = buffer.read;
	printf("\nPRINT_QUEUE\nCURRENT BUFFER\n");
	while (curr->next != buffer.read) {
		if (i < buffer.length) {
			printf("[%.1s]  ", curr->data);
		}
		else {
			printf("[ ]  ");
		}
		curr = curr->next;
		i++;
	}
	printf("\n");
}