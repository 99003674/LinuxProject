
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#define SIGHUP 1
#define SIGINT 2
#define SIGQUIT 3

pthread_mutex_t m1=PTHREAD_MUTEX_INITIALIZER;

sem_t s1;
sem_t s2;
sem_t s3;

void sighup(); 
void sigint();
void sigquit();
void Get_Report1();
void Get_Report2();
void Get_Report3();

void Alert_the_Doctor() // function for alerting the doctor
{ 
  int pid;
  
     if ((pid = fork()) < 0)  // throw error
		{ 
		perror("fork"); 
		exit(1); 
		} 
		if (pid == 0)
		{	                    
		signal(SIGHUP, sighup); 
	    signal(SIGINT, sigint); 
		signal(SIGQUIT, sigquit); 
		for (;;); 
		} 
		else 
		{ 
		printf("\nPARENT: sending SIGHUP\n\n");  
		kill(pid, SIGHUP); 
        sleep(2); 
		printf("\nPARENT: sending SIGINT\n\n"); 
		kill(pid, SIGINT); 
		sleep(2);
		printf("\nPARENT: sending SIGQUIT\n\n"); 
		kill(pid, SIGQUIT); 
		sleep(2); 
		} 
}

int Check_the_Vitals( int Breathing_rate, int Heart_beat)    // function for checking the vitals
{
	printf("\nVital Details : Heart beat - %d and Breathing rate - %d\n", Heart_beat, Breathing_rate);
	if(Breathing_rate > 15 && Breathing_rate<22 )
		{
			if (Heart_beat>65 && Heart_beat<105)
				{
					printf("\nPatient is normal\n");
				}
			else 
				{
					printf("\nHeart beat not in correct range - Alert the doctor\n");
					 Alert_the_Doctor();     // function calling when heartbeat is critical
				}
		}
		else
			{
				printf("\nPatient is in a critical situation - Alert the doctor \n");
				Alert_the_Doctor();	
			}
			
}
		

   
void sighup() 
  
{ 
    signal(SIGHUP, sighup); //reset signal
    printf("Doctor alerted\n"); 
} 
  
void sigint() 
  
{ 
    signal(SIGINT, sigint); //reset signal
    printf("Doctor is treating the patient \n"); 
} 
  
void sigquit() 
{ 
    printf("Treatment is complete\n"); 

}  

// patient 1

void* pat1(void* pv)
{
	sem_wait(&s1);
	pthread_mutex_lock(&m1);
	char str[]= "Mohan";
	int IDn= 101;
	printf("\nPatient name : %s \n Patient ID : %d", str,IDn);
	Check_the_Vitals(15,65);   //checking vitals
	pthread_mutex_unlock(&m1);
	sem_post(&s1);
	sem_post(&s2);
	//pthread_exit(NULL);

}
// patient 2
void* pat2(void* pv)
{
	sem_wait(&s2);
	pthread_mutex_lock(&m1);
	char str[]= "Binod";
	int IDn= 102;
	printf("\nPatient name : %s \n Patient ID : %d", str,IDn);
	Check_the_Vitals(10,110);    //checking vitals
	pthread_mutex_unlock(&m1);
	sem_post(&s2);
	sem_post(&s3);
    //pthread_exit(NULL);

}
// patient 3
void* pat3(void* pv)
{
	sem_wait(&s3);
	pthread_mutex_lock(&m1);
	char str[]= "Aditya";
	int IDn= 103;
	printf("\nPatient name : %s \n Patient ID : %d", str,IDn);
	Check_the_Vitals(25,50);     //checking vitals
	pthread_mutex_unlock(&m1);
	sem_post(&s3);

	//pthread_exit(NULL);
}

int main()
{

	// Creating thread handles
	pthread_t pt1;
	pthread_t pt2;
	pthread_t pt3;	   
	sem_init(&s1,0,1);
	sem_init(&s2,0,0);
	sem_init(&s3,0,0);


	// Creating threads
	pthread_create(&pt1,NULL,pat1,NULL);
	pthread_create(&pt2,NULL,pat2,NULL);
	pthread_create(&pt3,NULL,pat3,NULL);
	pthread_join(pt1,NULL);
	pthread_join(pt2,NULL);
	pthread_join(pt3,NULL);
	sem_destroy(&s1);
	sem_destroy(&s2);
	sem_destroy(&s3);
	pthread_mutex_destroy(&m1);
	return 0;	          
}
