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
	unsigned long MyTicket;					// If there is a chair in the waiting room patient thread will take a queue ticket
	unsigned long ID=pthread_self();		// used to print the Patient Thread ID	
	pthread_mutex_lock (&DRsOffice);			// Will lock the mutex to check what the patient should do, depending on the available chairs and DRs
	int TODO=0;								// Determines the Patient thread tasks
	if (AvailableChairs != 0){				//there is atleast one chair available, so patient can be admitted
		AvailableChairs--;					// Take a chair, to update the waiting room number of available chairs
		if (AvailableChairs==WRSize){		// Waiting room is empty, so should check whether any DR is sleeping or no
			if (SleepingDR !=0){			// Atleast one DR is sleeping. Take a ticket and wake up a DR, then wait to be called 
				TODO=1;
				SleepingDR--;				// It is assumed that there is one less sleeping DR, as the next Patient thread accessing the Mutex should see that change
			}
			else{
				TODO=2;						// Waiting room is empty but all DRs are visiting other patients; so there is no need fo wakeup call, wait for your ticket call
			}
		}
		else{								// There are other patients in the queue a head of you, take a ticket and wait for the ticket call
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
			pthread_mutex_unlock (&PQ . mutex);					 int NumDR, NumPatient, WRSize; 	// Variables are received from user and never change during the program; only used for comparison and decision making.
 volatile int SleepingDR = 0;		// Variable that holds the number of sleeping DRs. This variable is in DRsOffice Mutex.
 volatile int ReceptionDone = 0;	// The variable shows whether the Patient generation is done, or more patients are expected to be generated.
 volatile int NewPatientNum = 0;	// The variable holds the number of patients that get in the que while DRs are sleeping. This is a PQ.Mutex variable.
 volatile int AvailableChairs = 0;	// The variable shows available space in the wairing room and it is a DRsOffice mutex variable.
 
 /**
 The structure holds all the variables, mutex and condition variable that are realted to the Patient thread FIFO, i.e. Patient Queue.
 **/
 typedef struct Patient_Queue {
    pthread_cond_t cond;					// The condition variable of the structure mutex, to facilitate access to the que critical section
    pthread_mutex_t mutex;					// The mutex that control access to the critical data of Patient Que and prevent simultanious rea
    unsigned long queue_head, queue_tail;	// Variables of the Que that shows the last issued ticket number and the ticket that is called 
} Patient_Queue_t;
 
Patient_Queue_t PQ;
pthread_mutex_t DRsOffice	= PTHREAD_MUTEX_INITIALIZER;	// Mutext to control access to Office data variables including SleepingDR, AvailableChar, and ReceptionDone
pthread_cond_t	NewPatient	= PTHREAD_COND_INITIALIZER;		// Condition for the PQ.mutex that signals sleeping DR to wake up and call the next Patient
 
/**
This function is used to represent treating your patients
It generates a rndom number between MAX and MIN of visit time (4 and 8 in this case) and sleep the calling thread for that periode
**/
void treat_patient(void) {
	int range = MAX_VISIT-MIN_VISIT+1;
	long W_time;
	W_time= random();
	unsigned int Time=MIN_VISIT + W_time % range;
	sleep (Time);
	return;
}
 
 
/**
 * This thread is responsible for getting patients from the waiting room 
 * to treat and sleep when there are no patients. 
 * It is assumed that all patients will first taked a ticket entering the waiting room, i.e. queue, ans wake up DRs if there is one sleeping
 */
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
 
/**
 * This thread is responsible for acting as a patient, waking up doctors, waiting for doctors 
 * and be treated.
 */
void *patient_thread(void *arg) {
	unsigned long MyTicket;					// If there is a chair in the waiting room patient thread will take a queue ticket
	unsigned long ID=pthread_self();		// used to print the Patient Thread ID	
	pthread_mutex_lock (&DRsOffice);			// Will lock the mutex to check what the patient should do, depending on the available chairs and DRs
	int TODO=0;								// Determines the Patient thread tasks
	if (AvailableChairs != 0){				//there is atleast one chair available, so patient can be admitted
		AvailableChairs--;					// Take a chair, to update the waiting room number of available chairs
		if (AvailableChairs==WRSize){		// Waiting room is empty, so should check whether any DR is sleeping or no
			if (SleepingDR !=0){			// Atleast one DR is sleeping. Take a ticket and wake up a DR, then wait to be called 
				TODO=1;
				SleepingDR--;				// It is assumed that there is one less sleeping DR, as the next Patient thread accessing the Mutex should see that change
			}
			else{
				TODO=2;						// Waiting room is empty but all DRs are visiting other patients; so there is no need fo wakeup call, wait for your ticket call
			}
		}
		else{								// There are other patients in the queue a head of you, take a ticket and wait for the ticket call
			TODO=2;
		}
	}
	else{
		TODO=3;								// There is not any available chair in the vaiting room, the patient should leave the office without getting into queue or being served
	}
 
	pthread_mutex_unlock(&DRsOffice);		// There is no need to access DRsOffice variables, unlock the mutex and let others use the resources
	switch (TODO){
		case 1:								// Wake up call for a DR is required
			pthread_mutex_lock (&PQ . mutex);							// lock the PQ.mutex to access queue data
			MyTicket=PQ.queue_tail++;									// Take a ticket and go to the queue
			printf("Patient %lu takes ticket #%lu \n", ID, MyTicket);	// Let user know that the Patient thread is admitted with a specific ticket
			NewPatientNum++;											// indicated tha you have arrived
			pthread_cond_signal (&NewPatient);							// try to wakeup a DR sleeping on the NewPatient condition by sending a signal
			while (MyTicket != PQ . queue_head){						// Whait untill your ticket is called
				pthread_cond_wait(&PQ . cond, & PQ . mutex);			// Wait on the condition signal to indicate a new ticket number call, then check your ticket against the new ticket number
			}															// Loop will break when the Patient Thread ticket number is called
			pthread_mutex_unlock (&PQ . mutex);						// Unlock the mutex as the patient thread is done
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
		case 3:								// The thread cannot be served, just leave without getting into the queue
			printf("Patient %lu will leave; waiting room is full. \n", ID);	// Let user know that the Patient thread cannot be admitted and will leave
			break;		
	}
	pthread_exit (NULL);
	return NULL;
}
 
int main(int argc, char **argv) {
 
	//TODO: Define set-up required
	NumPatient=10;
	NumDR=3;
	WRSize=4;											// Initialize the variables until receive their value
    printf("#doctors %d, #patients %d, #chairs %d \n", NumDR, NumPatient, WRSize);
 
	if(argc != 4){														// Check the input send a message as an error or store the variables' value
		printf("Usage: DoctorsOffice <waitingSize> <patients> <doctors>\n");
		exit(0);
	}
	else {																
		WRSize	=	AvailableChairs	=	atoi(argv[1]);					// Size of waiting room and available chairs are the same at the begining as there is no Patient on the queue
		NumPatient	=atoi(argv[2]);										// Total patients that should be generated
		NumDR	=atoi(argv[3]);											// The number of DRs that should be generated
	}
	if ((NumDR==0)||(NumPatient==0)){									// the cinic run is useless without DRs and or patients
		printf("Error: There should be atleast one Doctor and one patient for the program to start \n");
		exit(0);
	}
    printf("#doctors %d, #patients %d, #chairs %d \n", NumDR, NumPatient, WRSize);
 
	/* Patient Queue initialization */
	pthread_cond_init (&PQ.cond, NULL);									// initialize the condition variable to defaults
	pthread_mutex_init (&PQ.mutex, NULL);								// Initialize the mutex to defaults
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
 
