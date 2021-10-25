#include <linux/kernel.h>
#include <linux/syscalls.h>

#define HALF_SIZE_BUFFER 10
struct ring_buffer_421 BUFF;

/* Initializes a circular buffer with nodes
*  that hold an int. Tail node points to head
*/
SYSCALL_DEFINE0(init_buffer_421) {
    // Buffer already initialized
    if (BUFF.read != NULL) {
        return -1;
    }

    int i;
    struct node_421* curr;
    struct node_421* prev;

    for (i = 0; i < SIZE_OF_BUFFER; i++) {
        // Create new node for each index
        struct node_421* n_node = (struct node_421*)kmalloc(sizeof(node >
            n_node->data = 0;
        n_node->next = NULL;
        curr = n_node;

        // First insertion
        // Initialize read/write pointers
        if (i == 0) {
            BUFF.read = curr;
            BUFF.write = curr;
            prev = curr;
        }

        // Last insertion
        // Set node to point to head, creates circle
        else if (i + 1 == SIZE_OF_BUFFER) {
            prev->next = curr;
            curr->next = BUFF.read;
        }
    }

    return 0;
}

