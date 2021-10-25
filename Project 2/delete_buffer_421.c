#include <linux/kernel.h>
#include <linux/syscalls.h>

/* Deallocates all memory used in circular buffer.
*  Fails if buffer is uninitialized.
*/
SYSCALL_DEFINE0(delete_buffer_421) {
    // Fails if buffer uninitialized
    if (BUFF.read != NULL) {
        struct node_421* curr;
        struct node_421* prev;
        curr = BUFF.read;
        int i;

        for (i = 0; i < SIZE_OF_BUFFER; i++) {
            prev = curr;
            curr = curr->next;
            if (prev) {
                kfree(prev);
            }
        }
        return 0;
    }
    return -1;
}