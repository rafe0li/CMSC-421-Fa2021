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

long init_buffer_syscall(void) {
        return syscall(__NR_init_buffer_421);
}

long insert_buffer_syscall(int i) {
        return syscall(__NR_insert_buffer_421);
}

long print_buffer_421(void) {
        return syscall(__NR_print_buffer_421);
}


long delete_buffer_421(void) {
        return syscall(__NR_print_buffer_421);
}

int main(int argc, char* argv[]) {
    long call0;
    long call1;
    long call2;
    long call10;
    long call11;

    call0 = init_buffer_syscall();

    if (call0 < 0) {
        perror("init_buffer syscall failed")
    }
    else {
        printf("init_buffer ran succesfully, check dmesg output\n");
    }

    call1 = insert_buffer_syscall(1);

    if (call1 < 0) {
        perror("insert_buffer syscall failed")
    }
    else {
        printf("insert_buffer ran succesfully, check dmesg output\n");
    }

    call2 = insert_buffer_syscall(1);

    if (call2 < 0) {
        perror("insert_buffer syscall failed")
    }
    else {
        printf("insert_buffer ran succesfully, check dmesg output\n");
    }

    call3 = insert_buffer_syscall(1);

    if (call3 < 0) {
        perror("insert_buffer syscall failed")
    }
    else {
        printf("insert_buffer ran succesfully, check dmesg output\n");
    }

    call4 = insert_buffer_syscall(1);

    if (call4 < 0) {
        perror("insert_buffer syscall failed")
    }
    else {
        printf("insert_buffer ran succesfully, check dmesg output\n");
    }

    call10 = print_buffer_syscall();

    if (call10 < 0) {
        perror("print_buffer syscall failed")
    }
    else {
        printf("print_buffer ran succesfully, check dmesg output\n");
    }

    call11 = delete_buffer_syscall();

    if (call11 < 0) {
        perror("delete_buffer syscall failed")
    }
    else {
        printf("delete_buffer ran succesfully, check dmesg output\n");
    }

    return 0;
}