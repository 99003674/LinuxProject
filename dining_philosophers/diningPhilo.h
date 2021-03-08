//header file for dining philosophers//

#ifndef __DININGPHILO_H__
#define __DININGPHILO_H__

#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h> 

void check(int phnum);
void take_chopstick(int phnum) ;
void put_chopstick(int phnum);
void* philospher(void* num);

#endif
