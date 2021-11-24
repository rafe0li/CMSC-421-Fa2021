/*
* @authors Rafael Li, rafaell1@umbc.edu and Kelly Snyder, ks16@umbc.edu
* @buffer_user.c
* KERNEL VERSION
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
static struct semaphore mutex;
static struct semaphore fill_count;
static struct semaphore empty_count;

/*
 * Initializes buffer.
 */
SYSCALL_DEFINE0(init_buffer_421) {
	// Create the root node.
	node_421_t* node;
	node_421_t* curr;
	int i;

	// Cannot initialize already initialized buffer
	if (buffer.read || buffer.write) {
		printk("init_buffer_421(): Buffer already exists. Aborting.\n");
		return -1;
	}

	node = (node_421_t*)kmalloc(sizeof(node_421_t), GFP_KERNEL);
	// Create the rest of the nodes, linking them all together.
	curr = node;
	// Note that we've already created one node, so i = 1.
	for (i = 1; i < SIZE_OF_BUFFER; i++) {
		curr->next = (node_421_t*)kmalloc(sizeof(node_421_t), GFP_KERNEL);
		curr = curr->next;
	}
	// Complete the chain.
	curr->next = node;
	buffer.read = node;
	buffer.write = node;
	buffer.length = 0;

	// Initialize semaphores
	sema_init(&mutex, 1);
	sema_init(&fill_count, 0);
	sema_init(&empty_count, 20);

	return 0;
}

/*
 * Enqueues given value into buffer.
 * @param[in] data char* to be enqueued
 */
SYSCALL_DEFINE1(enqueue_buffer_421, char*, data) {
	// Cannot enqueue uninitialized buffer
	if (!buffer.write) {
		printk("enqueue_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}

	// Wait for access to buffer and signal from dequeue if buffer full
	down(&empty_count);
	down(&mutex);

	// Allocates memory for char* block and copies
	memset(buffer.write->data, 0, DATA_LENGTH);
	copy_from_user(buffer.write->data, data, DATA_LENGTH);

	// Advance the pointer.
	buffer.write = buffer.write->next;
	buffer.length++;

	// Unlocks buffer and signals dequeue
	up(&mutex);
	up(&fill_count);

	return 0;
}

/*
 * Dequeues head of buffer.
 * @param[in] data char* that is dequeued
 */
SYSCALL_DEFINE1(dequeue_buffer_421, char*, data) {
	node_421_t* curr;
	node_421_t* prev;

	// Cannot dequeue uninitialized buffer
	if (!buffer.write) {
		printk("dequeue_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}

	// Wait for access to buffer and signal from enqueue if buffer empty
	down(&fill_count);
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

	// Unlocks buffer and signals enqueue
	up(&mutex);
	up(&empty_count);

	return 0;
}

/*
 * Deallocates buffer.
 */
SYSCALL_DEFINE0(delete_buffer_421) {
	// Get rid of all existing nodes.
	node_421_t* temp;
	node_421_t* curr;

	// Cannot delete uninitialized buffer
	if (!buffer.read) {
		printk("delete_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	
	curr = buffer.read->next;
	while (curr != buffer.read) {
		temp = curr->next;
		kfree(curr);
		curr = temp;
	}
	// Free the final node.
	kfree(curr);
	curr = NULL;
	// Reset the buffer.
	buffer.read = NULL;
	buffer.write = NULL;
	buffer.length = 0;

	// Delete semaphores
	sema_destroy(&mutex);
	sema_destroy(&fill_count);
	sema_destroy(&empty_count);

	return 0;
}

/*
 * Prints all values in nodes, does not read nonexistent entries.
 */
SYSCALL_DEFINE0(print_buffer_421) {
	int i = 0;
	node_421_t* curr;

	// Cannot print uninitialized buffer
	if (!buffer.read) {
		printk("print_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}

	// Prints queue
	printk("\nPRINT_QUEUE(): CURRENT BUFFER\n");
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