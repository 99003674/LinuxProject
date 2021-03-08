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
	bool finished	=	false;						// Variable that indicates there is no more work to be done; it breaks the loop of DR thread
	unsigned long ID=pthread_self();				// The specific DR_thread ID
	while(!finished) {								//exit your doctor threads when no more patients coming (amount specified on cammond line)
		pthread_mutex_lock (&DRsOffice);				// DR Thread lock DRsOffice mutex to read/change available chairs and sleeping DRs or check ReceptionDone
		if (AvailableChairs==WRSize){				// Waiting room is empty and no body is on the que
			if (ReceptionDone){						// No More Patient will be generated for the run --. finished
				pthread_mutex_unlock (&DRsOffice);	// Unlock the mutex to let other DRs use it.
				finished=true;						// Signale finished to break the loop and exit the thread
			}
			else{									// Waiting room is empty, but more patients are expected to visit the office. Sleep until a new patient wakes you up
				SleepingDR++;						// Indicate that there is one more available DR
				printf ("@user: Doctor Thread #%lu is going to sleep #sleeping DRs %d", ID, SleepingDR);
				pthread_mutex_unlock (&DRsOffice);	// Unlock the office as non of its variables need to be checked or changed
				pthread_mutex_lock (&PQ . mutex);		// Lock the que mutex to access its variable and wake up by its signal
				while (NewPatientNum==0){			// While there is not any new patient in the que
					pthread_cond_wait (&NewPatient,&PQ. mutex);		// Wait on the PQ.mutex on the condition variable of NewPatient signal
				}									// When NewPatient signal is isuued the loop will break, otherwise another DR has taken the patient
				NewPatientNum--;					// As the DR will visit the patient thread, the number of new patients should decrease
				PQ.queue_head++;					// The ticket number that will be called is updated
				pthread_cond_broadcast (&PQ.cond);	// The change on calling ticket is broadcated, patients will check the new queue_head with their ticket number on the patient thread
				printf (" DR %lu will visit the patient with ticket queue %lu", ID, PQ.queue_head); 	// Print a message for the indicating the DR thread ID and the ticket it will serve
				pthread_mutex_unlock (&PQ.mutex);	// unlock the Mutex to allow other threads in the line to access its data
				treat_patient();					// sleep the calling thread for a random time between 4-8 seconds
			}
		}
		else{										// Threa is atleast one patient in the waiting room to be visited so next ticket number should be called
			pthread_mutex_unlock(&DRsOffice);		// The lock is not required unlock mutex so that other threads can lock it
			pthread_mutex_lock (&PQ. mutex);		// Lock the PQ.mutex to access the que variables and data
			PQ.queue_head++;						// increase the header to call the next ticket number
			pthread_cond_broadcast (&PQ.cond);		// Broadcast the change with the PQ.cond, so that all waiting threads on that condition check their ticket number againt the new header
			printf (" DR %lu will visit the patient with ticket queue %lu", ID, PQ.queue_head);	// Inform the user that a specific DR Thread has called a patient with the respective ticket number
			pthread_mutex_unlock (&PQ.mutex);		// Unlock the mutext to let waiting threads lock it 
			treat_patient();
		}
	}
	pthread_exit(NULL);								// Exit the thread to join
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
	PQ.queue_head	=0;													// set queu head to zero as no ticket is called at the begining
	PQ.queue_tail	=0;													// set queue tail to zero az no ticket is generated
 
	/* threads of DRs and Patients*/ 
	pthread_t	Doctor	[NumDR];		
	pthread_t	Patient	[NumPatient];
 
	// Generate Doctor threads
	pthread_mutex_lock	(&DRsOffice);									// Lock the mutex until all DRs are generated
	int	count	=0;
	for (count=0; count < NumDR; ++count){								// create threads in a loop
		pthread_create ( &Doctor[count], NULL, doctor_thread, NULL);	// create Doctor threads with specific ID and the function doctor_thread
		printf ("@user: Doctor Thread #%d is created", count);
	}
	pthread_mutex_unlock (&DRsOffice);									// unlock the mutex and let DR threads to proceed
 
	// Generate Patient Threads
	for (count=0; count < NumPatient; ++count){							// create threads in a loop
		pthread_create ( &Patient[count], NULL, patient_thread, NULL);	// create Patient threads with specific ID and the function patient_thread
		printf ("@user: Patient Thread #%d is created", count);
		int range = MAX_SLEEP-MIN_SLEEP+1;								// set the range of the random sleep time
		long W_time;
		W_time= random();												// generate a random number
		unsigned int Time=MIN_SLEEP + W_time % range;					// generate a random number between 1-5 seconds
		printf ("@user: Sleep time #%d is", count);
		sleep (Time);													// sleep the main thread for the random period
	}
	pthread_mutex_lock (&DRsOffice);										// lock the mutex to change ReceptionDone flag
	ReceptionDone=1;													// change the flag to indicate that all patient threads are created
	pthread_mutex_unlock (&DRsOffice);									// unlock mutex so other threads can lock it
 
	//TODO: Clean up
	for (count=0; count<NumPatient; ++count){							// Join all patient threads upon their exits
		pthread_join (Patient[count], NULL);
	}
	for (count=0; count<NumDR; ++count){								// Join all doctor threads upon their exits
		pthread_join (Doctor[count], NULL);
	}
	pthread_mutex_destroy (&PQ.mutex);									// destroy the mutex
	pthread_mutex_destroy (&DRsOffice);									// destroy the mutex
	return 0;	
}
			printf("Patient %lu is called \n", ID);
			break;
		case 2:								// Just wait in the queue, no wake up call
			pthread_mutex_lock (&PQ . mutex);							// lock the PQ.mutex to access queue data
			MyTicket=PQ.queue_tail++;									// Take a ticket and go to the queue
			printf("Patient %lu takes ticket #%lu \n", ID, MyTicket);	// Let user know that the Patient thread is admitted with a specific ticket
			while (MyTicket != PQ . queue_head){						// Whait untill your ticket is called
				pthread_cond_wait(&PQ . cond, &PQ . mutex);			// Wait on the condition signal to indicate a new ticket number call, then check your ticket against the new ticket number
			}															// Loop will break when the Patient Thread ticket number is called
			pthread_mutex_unlock (&PQ . mutex);						// Unlock the mutex as the patient thread is done
			printf("Patient %lu is called \n", ID);
			break;
		case 3:							
			printf("Patient %lu will leave; waiting room is full. \n", ID);	
			break;		
	}
	pthread_exit (NULL);
	return NULL;
}
 
