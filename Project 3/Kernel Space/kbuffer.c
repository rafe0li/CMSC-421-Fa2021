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
#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include "buffer.h"
#include "buffer_util.h"

static ring_buffer_421_t buffer;
struct semaphore mutex;
struct semaphore fill_count;
struct semaphore empty_count;

/*
 * Initializes buffer.
 */
SYSCALL_DEFINE0(init_buffer_421) {
	// Ensure we're not initializing a buffer that already exists.
	if (buffer.read || buffer.write) {
		printk("init_buffer_421(): Buffer already exists. Aborting.\n");
		return -1;
	}

	// Create the root node.
	node_421_t* node;
	node = (node_421_t*)kmalloc(sizeof(node_421_t));
	// Create the rest of the nodes, linking them all together.
	node_421_t* curr;
	int i;
	curr = node;
	// Note that we've already created one node, so i = 1.
	for (i = 1; i < SIZE_OF_BUFFER; i++) {
		curr->next = (node_421_t*)kmalloc(sizeof(node_421_t));
		curr = curr->next;
	}
	// Complete the chain.
	curr->next = node;
	buffer.read = node;
	buffer.write = node;
	buffer.length = 0;

	// Initialize semaphores
	sema_init(&mutex, 1);
	sema_init(&fill_count, 1);
	sema_init(&empty_count, 1);

	return 0;
}

/*
 * Enqueues given value into buffer.
 * @param[in] data char* to be enqueued
 */
SYSCALL_DEFINE1(enqueue_buffer_421, char*, data) {
	if (!buffer.write) {
		printk("enqueue_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}

	// If buffer empty, block and signal enqueue
	if (buffer.length == 20) {
		up(&empty_count);
		down(&fill_count);
	}

	// Mutex ensures exclusive access to buffer
	down(&mutex);

	// Allocates memory for char* block and copies
	memset(buffer.write->data, 0, DATA_LENGTH);
	copy_from_user(buffer.write->data, data, DATA_LENGTH);

	// Advance the pointer.
	buffer.write = buffer.write->next;
	buffer.length++;

	// Buffer unlocked and thread waiting for buffer signaled
	up(&mutex);

	// If the last enqueue filled the buffer, signal dequeue
	if (buffer.length == 20) {
		up(&empty_count);
	}

	return 0;
}

/*
 * Dequeues head of buffer.
 * @param[in] data char* that is dequeued
 */
SYSCALL_DEFINE1(dequeue_buffer_421, char*, data) {
	node_421_t* curr;
	node_421_t* prev;

	// Empty buffer condition
	if (!buffer.write) {
		printk("dequeue_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}

	// If buffer empty, signal enqueue and wait
	if (buffer.length == 0) {
		up(&fill_count);
		down(&empty_count);
	}

	down(&mutex);

	// Copies data from buffer into param
	copy_to_user(data, buffer.read->data, DATA_LENGTH);
	buffer.length--;

	// Moves all values up in queue
	curr = buffer.read;
	while (curr->next != buffer.read) {
		prev = curr;
		curr = curr->next;
		memcpy(prev->data, curr->data, DATA_LENGTH);
	}

	up(&mutex);

	// If the last dequeue emptied the buffer, signal enqueue
	if (buffer.length == 0) {
		up(&fill_count);
	}

	return 0;
}

/*
 * Deallocates buffer.
 */
SYSCALL_DEFINE0(delete_buffer_421) {
	if (!buffer.read) {
		printk("delete_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	// Get rid of all existing nodes.
	node_421_t* temp;
	node_421_t* current = buffer.read->next;
	while (current != buffer.read) {
		temp = current->next;
		kfree(current);
		current = temp;
	}
	// Free the final node.
	kfree(current);
	current = NULL;
	// Reset the buffer.
	buffer.read = NULL;
	buffer.write = NULL;
	buffer.length = 0;
	return 0;
}

/*
 * Prints all values in nodes, does not read nonexistent entries.
 */
SYSCALL_DEFINE0(print_buffer_421) {
	if (!buffer.read) {
		printk("print_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	
	int i = 0;
	node_421_t* curr = buffer.read;
	printk("\nPRINT_QUEUE\nCURRENT BUFFER\n");
	while (curr->next != buffer.read) {
		if (i < buffer.length) {
			printk("[%.1s]  ", curr->data);
		}
		else {
			printk("[ ]  ");
		}
		curr = curr->next;
		i++;
	}
	printk("\n");

	return 0;
}