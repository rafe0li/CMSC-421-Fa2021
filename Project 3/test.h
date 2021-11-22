#ifndef TEST_H
#define TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __KERNEL__
#include <semaphore.h>
#else
#include <linux/semaphore.h>
#endif

#ifndef __KERNEL__
void producer(char*);
void consumer();

#ifdef __cplusplus
}
#endif

#endif

#endif