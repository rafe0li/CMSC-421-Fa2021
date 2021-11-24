/*
* @author Rafael Li, rafaell1@umbc.edu
* @test.h
* Header for producer/consumer threading process.
*
*/
#ifndef TEST_H
#define TEST_H

#ifdef __cplusplus
extern "C" {
#endif

void *produce(void*);
void *consume(void*);

#ifdef __cplusplus
}
#endif

#endif