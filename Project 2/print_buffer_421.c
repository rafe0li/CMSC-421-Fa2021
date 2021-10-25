#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <buffer.h>

#define HALF_SIZE_BUFFER 10

/* Prints all nodes in the buffer.
*  Fails if buffer is uninitialized.
*/
SYSCALL_DEFINE0(print_buffer_421) {
    // Fails if buffer already initialized
    if (BUFF.read == NULL) {
        return -1;
    }

    int i;
    struct node_421* curr;
    curr = BUFF.read;

    // Prints first half on one line, second half on next
    for (i = 0; i < HALF_SIZE_BUFFER; i++) {
        printf(" %d. [%d]\t", i + 1, curr->data);
        curr = curr->next;
    }

    printf("\n");

    for (i = HALF_SIZE_BUFFER; i < SIZE_OF_BUFFER; i++) {
        printf("%d. [%d]\t", i + 1, curr->data);
        curr = curr->next;
    }

    printf("\n\n");

    return 0;

}