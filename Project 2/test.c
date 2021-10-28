#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <linux/kernel.h>
#include <sys/syscall.h>

#define __NR_init_buffer_421 442
#define __NR_insert_buffer_421 443
#define __NR_print_buffer_421 444
#define __NR_delete_buffer_421 445

long sys_init_buffer_421(void) {
        return syscall(__NR_init_buffer_421);
}

long sys_insert_buffer_421(int i) {
        return syscall(__NR_insert_buffer_421, i);
}

long sys_print_buffer_421(void) {
        return syscall(__NR_print_buffer_421);
}

long sys_delete_buffer_421(void) {
        return syscall(__NR_delete_buffer_421);
}

int main(int argc, char* argv[]) {
    long call;

    // Test inserting into an uninitialized buffer
    call = sys_insert_buffer_421(1);
    if (call < 0) {
        perror("ERROR: Buffer is full or uninitialized, cannot insert");
    }
    else {
        printf("insert_buffer ran successfully, check dmesg output\n");
    }

    // Test printing a uninitialized buffer
    call = sys_print_buffer_421();
    if (call < 0) {
        perror("ERROR: Buffer uninitialized, cannot print");
    }
    else {
        printf("print_buffer ran successfully, check dmesg output\n");
    }

    // Testing deleting a uninitialized buffer
    call = sys_delete_buffer_421();
    if (call < 0) {
        perror("ERROR: Buffer is uninitialized, cannot delete");
    }
    else {
        printf("delete_buffer ran successfully, check dmesg output\n");
    }



    // Test init
    call = sys_init_buffer_421();
    if (call < 0) {
        perror("ERROR: Buffer already initialized, cannot initialize");
    }
    else {
        printf("init_buffer ran successfully, check dmesg output\n");
    }

    // Test initializing over already initialized buffer
    call = sys_init_buffer_421();

    if (call < 0) {
        perror("ERROR: Buffer already initialized, cannot initialize");
    }
    else {
        printf("init_buffer ran successfully, check dmesg output\n");
    }

    // Test insert, fills buffer
    int i;
    for (i = 0; i < 20; i++) {
        call = sys_insert_buffer_421(i);
        if (call < 0) {
            perror("ERROR: Buffer is full or uninitialized, cannot insert");
        }
        else {
            printf("insert_buffer ran successfully, check dmesg output\n");
        }
    }



    // Test inserting into full buffer
    call = sys_insert_buffer_421(1);
    if (call < 0) {
        perror("ERROR: Buffer is full or uninitialized, cannot insert");
    }
    else {
        printf("insert_buffer ran successfully, check dmesg output\n");
    }

    // Test print
    call = sys_print_buffer_421();
    if (call < 0) {
        perror("ERROR: Buffer uninitialized, cannot print");
    }
    else {
        printf("print_buffer ran successfully, check dmesg output\n");
    }

    // Test delete
    call = sys_delete_buffer_421();
    if (call < 0) {
        perror("ERROR: Buffer is uninitialized, cannot delete");
    }
    else {
        printf("delete_buffer ran successfully, check dmesg output\n");
    }

    // Test inserting into deleted buffer
    call = sys_insert_buffer_421(1);
    if (call < 0) {
        perror("ERROR: Buffer is full or uninitialized, cannot insert");
    }
    else {
        printf("insert_buffer ran successfully, check dmesg output\n");
    }

    // Test printing a deleted buffer
    call = sys_print_buffer_421();
    if (call < 0) {
        perror("ERROR: Buffer uninitialized, cannot print");
    }
    else {
        printf("print_buffer ran successfully, check dmesg output\n");
    }

    // Test deleting a deleted buffer
    call = sys_delete_buffer_421();
    if (call < 0) {
        perror("ERROR: Buffer is uninitialized, cannot delete");
    }
    else {
        printf("delete_buffer ran successfully, check dmesg output\n");
    }

    return 0;
}