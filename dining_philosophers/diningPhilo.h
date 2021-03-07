#ifndef __DININGPHILO_H__
#define __DININGPHILO_H__

#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h> 

void test(int phnum);
void take_fork(int phnum) ;
void put_fork(int phnum);
void* philospher(void* num);



#endif
