#include"Patientdoctor.h"

int NumDR, NumPatient, WRSize; 	
 volatile int SleepingDR = 0;		
 volatile int ReceptionDone = 0;	
 volatile int NewPatientNum = 0;	
 volatile int AvailableChairs = 0;	
 

 typedef struct Patient_Queue {
    pthread_cond_t cond;					
    pthread_mutex_t mutex;				
    unsigned long queue_head, queue_tail;	
} Patient_Queue_t;
 
Patient_Queue_t PQ;
pthread_mutex_t DRsOffice	= PTHREAD_MUTEX_INITIALIZER;	
pthread_cond_t	NewPatient	= PTHREAD_COND_INITIALIZER;		
 

void treat_patient(void) {
	int range = MAX_VISIT-MIN_VISIT+1;
	long W_time;
	W_time= random();
	unsigned int Time=MIN_VISIT + W_time % range;
	sleep (Time);
	return;
}
 
 

void *doctor_thread(void *arg) {
 
	
	bool finished	=	false;						
	unsigned long ID=pthread_self();			
	while(!finished) {							
		pthread_mutex_lock (&DRsOffice);				
		if (AvailableChairs==WRSize){			
			if (ReceptionDone){						
				pthread_mutex_unlock (&DRsOffice);	
				finished=true;					
			}
			else{								
				SleepingDR++;						
				printf ("@user: Doctor Thread #%lu is going to sleep #sleeping DRs %d", ID, SleepingDR);
				pthread_mutex_unlock (&DRsOffice);	
				pthread_mutex_lock (&PQ . mutex);		
				while (NewPatientNum==0){			
					pthread_cond_wait (&NewPatient,&PQ. mutex);		
				}									
				NewPatientNum--;					
				PQ.queue_head++;					
				pthread_cond_broadcast (&PQ.cond);	
				printf (" DR %lu will visit the patient with ticket queue %lu", ID, PQ.queue_head); 	
				pthread_mutex_unlock (&PQ.mutex);	
				treat_patient();					
			}
		}
		else{										
			pthread_mutex_unlock(&DRsOffice);		
			pthread_mutex_lock (&PQ. mutex);		
			PQ.queue_head++;						
			pthread_cond_broadcast (&PQ.cond);		
			printf (" DR %lu will visit the patient with ticket queue %lu", ID, PQ.queue_head);	
			pthread_mutex_unlock (&PQ.mutex);		
			treat_patient();
		}
	}
	pthread_exit(NULL);							
	return NULL;
}
 
void *patient_thread(void *arg) {
	unsigned long MyTicket;					
	unsigned long ID=pthread_self();		
	pthread_mutex_lock (&DRsOffice);			
	int TODO=0;								
	if (AvailableChairs != 0){				
		AvailableChairs--;					
		if (AvailableChairs==WRSize){		
			if (SleepingDR !=0){			
				TODO=1;
				SleepingDR--;				
			}
			else{
				TODO=2;						
			}
		}
		else{								
			TODO=2;
		}
	}
	else{
		TODO=3;								
	}
 
	pthread_mutex_unlock(&DRsOffice);		
	switch (TODO){
		case 1:								
			pthread_mutex_lock (&PQ . mutex);							
			MyTicket=PQ.queue_tail++;								
			printf("Patient %lu takes ticket #%lu \n", ID, MyTicket);	
			NewPatientNum++;										
			pthread_cond_signal (&NewPatient);							
			while (MyTicket != PQ . queue_head){					
				pthread_cond_wait(&PQ . cond, & PQ . mutex);			
			}															
			pthread_mutex_unlock (&PQ . mutex);				
int NumDR, NumPatient, WRSize; 	
 volatile int SleepingDR = 0;		
 volatile int ReceptionDone = 0;	
 volatile int NewPatientNum = 0;	
 volatile int AvailableChairs = 0;	
 

 typedef struct Patient_Queue {
    pthread_cond_t cond;					
    pthread_mutex_t mutex;					
    unsigned long queue_head, queue_tail;
} Patient_Queue_t;
 
Patient_Queue_t PQ;
pthread_mutex_t DRsOffice	= PTHREAD_MUTEX_INITIALIZER;	
pthread_cond_t	NewPatient	= PTHREAD_COND_INITIALIZER;	
 

void treat_patient(void) {
	int range = MAX_VISIT-MIN_VISIT+1;
	long W_time;
	W_time= random();
	unsigned int Time=MIN_VISIT + W_time % range;
	sleep (Time);
	return;
}
 
 

void *doctor_thread(void *arg) {
 
	//TODO: Define set-up required
	bool finished	=	false;						
	unsigned long ID=pthread_self();				
	while(!finished) {								
		pthread_mutex_lock (&DRsOffice);				
		if (AvailableChairs==WRSize){				
			if (ReceptionDone){						
				pthread_mutex_unlock (&DRsOffice);	
				finished=true;						
			}
			else{									
				SleepingDR++;						
				printf ("@user: Doctor Thread #%lu is going to sleep #sleeping DRs %d", ID, SleepingDR);
				pthread_mutex_unlock (&DRsOffice);	
				pthread_mutex_lock (&PQ . mutex);		
				while (NewPatientNum==0){			
					pthread_cond_wait (&NewPatient,&PQ. mutex);		
				}									
				NewPatientNum--;					
				PQ.queue_head++;					
				pthread_cond_broadcast (&PQ.cond);	
				printf (" DR %lu will visit the patient with ticket queue %lu", ID, PQ.queue_head); 	 
				pthread_mutex_unlock (&PQ.mutex);	
				treat_patient();					
			}
		}
		else{										
			pthread_mutex_unlock(&DRsOffice);		
			pthread_mutex_lock (&PQ. mutex);		
			PQ.queue_head++;						
			pthread_cond_broadcast (&PQ.cond);		
			printf (" DR %lu will visit the patient with ticket queue %lu", ID, PQ.queue_head);	
			pthread_mutex_unlock (&PQ.mutex);		 
			treat_patient();
		}
	}
	pthread_exit(NULL);								
	return NULL;
}
 

void *patient_thread(void *arg) {
	unsigned long MyTicket;					
	unsigned long ID=pthread_self();		
	pthread_mutex_lock (&DRsOffice);			
	int TODO=0;								
	if (AvailableChairs != 0){				
		AvailableChairs--;					
		if (AvailableChairs==WRSize){		
			if (SleepingDR !=0){			
				TODO=1;
				SleepingDR--;			
			}
			else{
				TODO=2;					
			}
		}
		else{								
			TODO=2;
		}
	}
	else{
		TODO=3;								 
	}
 
	pthread_mutex_unlock(&DRsOffice);		
	switch (TODO){
		case 1:								
			pthread_mutex_lock (&PQ . mutex);							
			MyTicket=PQ.queue_tail++;									
			printf("Patient %lu takes ticket #%lu \n", ID, MyTicket);	
			NewPatientNum++;											
			pthread_cond_signal (&NewPatient);							
			while (MyTicket != PQ . queue_head){						
				pthread_cond_wait(&PQ . cond, & PQ . mutex);			
			}															
			pthread_mutex_unlock (&PQ . mutex);						
			printf("Patient %lu is called \n", ID);
			break;
		case 2:								
			pthread_mutex_lock (&PQ . mutex);							
			MyTicket=PQ.queue_tail++;									
			printf("Patient %lu takes ticket #%lu \n", ID, MyTicket);	
			while (MyTicket != PQ . queue_head){						
				pthread_cond_wait(&PQ . cond, &PQ . mutex);			
			}															
			pthread_mutex_unlock (&PQ . mutex);						
			printf("Patient %lu is called \n", ID);
			break;
		case 3:								
			printf("Patient %lu will leave; waiting room is full. \n", ID);	
			break;		
	}
	pthread_exit (NULL);
	return NULL;
}
 
int main(int argc, char **argv) {
 
	NumPatient=10;
	NumDR=3;
	WRSize=4;											
    printf("#doctors %d, #patients %d, #chairs %d \n", NumDR, NumPatient, WRSize);
 
	if(argc != 4){														 
		printf("Usage: DoctorsOffice <waitingSize> <patients> <doctors>\n");
		exit(0);
	}
	else {																
		WRSize	=	AvailableChairs	=	atoi(argv[1]);					
		NumPatient	=atoi(argv[2]);										
		NumDR	=atoi(argv[3]);											
	}
	if ((NumDR==0)||(NumPatient==0)){									
		printf("Error: There should be atleast one Doctor and one patient for the program to start \n");
		exit(0);
	}
    printf("#doctors %d, #patients %d, #chairs %d \n", NumDR, NumPatient, WRSize);
 
	/* Patient Queue initialization */
	pthread_cond_init (&PQ.cond, NULL);									
	pthread_mutex_init (&PQ.mutex, NULL);								
	PQ.queue_head	=0;													
	PQ.queue_tail	=0;													
 
	/* threads of DRs and Patients*/ 
	pthread_t	Doctor	[NumDR];		
	pthread_t	Patient	[NumPatient];
 
	// Generate Doctor threads
	pthread_mutex_lock	(&DRsOffice);									
	int	count	=0;
	for (count=0; count < NumDR; ++count){								
		pthread_create ( &Doctor[count], NULL, doctor_thread, NULL);	
		printf ("@user: Doctor Thread #%d is created", count);
	}
	pthread_mutex_unlock (&DRsOffice);									
 
	// Generate Patient Threads
	for (count=0; count < NumPatient; ++count){							
		pthread_create ( &Patient[count], NULL, patient_thread, NULL);	
		printf ("@user: Patient Thread #%d is created", count);
		int range = MAX_SLEEP-MIN_SLEEP+1;								
		long W_time;
		W_time= random();												
		unsigned int Time=MIN_SLEEP + W_time % range;					
		printf ("@user: Sleep time #%d is", count);
		sleep (Time);													
	}
	pthread_mutex_lock (&DRsOffice);										 
	ReceptionDone=1;													
	pthread_mutex_unlock (&DRsOffice);									
 
	//TODO: Clean up
	for (count=0; count<NumPatient; ++count){							
		pthread_join (Patient[count], NULL);
	}
	for (count=0; count<NumDR; ++count){								
		pthread_join (Doctor[count], NULL);
	}
	pthread_mutex_destroy (&PQ.mutex);									
	pthread_mutex_destroy (&DRsOffice);									
	return 0;	
}
			printf("Patient %lu is called \n", ID);
			break;
		case 2:								
			pthread_mutex_lock (&PQ . mutex);							
			MyTicket=PQ.queue_tail++;									
			printf("Patient %lu takes ticket #%lu \n", ID, MyTicket);	
			while (MyTicket != PQ . queue_head){						
				pthread_cond_wait(&PQ . cond, &PQ . mutex);			
			}															
			pthread_mutex_unlock (&PQ . mutex);						
			printf("Patient %lu is called \n", ID);
			break;
		case 3:							
			printf("Patient %lu will leave; waiting room is full. \n", ID);	
			break;		
	}
	pthread_exit (NULL);
	return NULL;
}
 
