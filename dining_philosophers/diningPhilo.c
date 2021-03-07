#include "diningPhilo.h"

#define NUMBER 5 
#define FIRST (phnum + 4) % NUMBER 
#define SECOND (phnum + 1) % NUMBER
#define THINK 2 
#define HUNGRY 1 
#define EAT 0 

int state[NUMBER]; 
int philosophers[NUMBER] = { 0, 1, 2, 3, 4 }; 

sem_t mutex; 
sem_t S[NUMBER]; 

//philosopher actions
void* philospher(void* num) 
{ 
	while (1)
	{ 
		int* i = num; 
		sleep(1); 
		take_chopstick(*i); 
		sleep(0); 
		put_chopstick(*i); 
	} 
} 

// taking chopsticks 
void take_chopstick(int phnum) 
{ 
	sem_wait(&mutex); 
	// state that hungry 
	state[phnum] = HUNGRY; 
	printf("Philosopher %d is Hungry\n", phnum + 1); 
	// eat if neighbours are not eating 
	check(phnum); 
	sem_post(&mutex); 
	// if unable to eat wait to be signalled 
	sem_wait(&S[phnum]); 
	sleep(1); 
} 

// put down chopsticks 
void put_chopstick(int phnum) 
{ 
	sem_wait(&mutex); 
	// state that thinking 
	state[phnum] = THINK; 
	printf("Philosopher %d putting fork %d and %d down\n", phnum + 1, FIRST + 1, phnum + 1); 
	printf("Philosopher %d is thinking\n", phnum + 1); 
	check(FIRST); 
	check(SECOND); 
	sem_post(&mutex); 
} 

void check(int phnum) 
{ 
	if (state[phnum] == HUNGRY && state[FIRST] != EAT && state[SECOND] != EAT) 
	{ 
		// eating state
		state[phnum] = EAT; 
		sleep(2); 
		printf("Philosopher %d takes fork %d and %d\n", phnum + 1, FIRST + 1, phnum + 1); 
		printf("Philosopher %d is Eating\n", phnum + 1); 
		sem_post(&S[phnum]); 
	} 
} 

int main() 
{ 
	int i; 
	pthread_t thread_id[NUMBER]; 
	// initialize the semaphores 
	sem_init(&mutex, 0, 1); 
	for (i = 0; i < NUMBER; i++) 
		sem_init(&S[i], 0, 0); 
	for (i = 0; i < NUMBER; i++) { 
		// create philosopher processes 
		pthread_create(&thread_id[i], NULL, philospher, &philosophers[i]); 
		printf("Philosopher %d is thinking\n", i + 1); 
	} 

	for (i = 0; i < NUMBER; i++) 
		pthread_join(thread_id[i], NULL); 
} 
