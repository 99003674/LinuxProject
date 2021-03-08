

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
