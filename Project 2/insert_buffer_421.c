#include <linux/kernel.h>
#include <linux/syscalls.h>

/* Inserts integer into buffer, increases length
*  and position of write pointer.
*  Fails if buffer is full.
*
* @param[in] i int to write into next node
*/
SYSCALL_DEFINE0(insert_buffer_421) {
    // Buffer full
    if (BUFF.read == BUFF.write) {
        return -1;
    }

    // Insert data, increment write pointer and length
    BUFF.write->data = i;
    BUFF.write = BUFF.write->next;
    BUFF.length++;
    return 0;
}