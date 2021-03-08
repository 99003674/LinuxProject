#include"Patientdoctor.h"
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
 
	
	pthread_cond_init (&PQ.cond, NULL);									
	pthread_mutex_init (&PQ.mutex, NULL);								
	PQ.queue_head	=0;													
	PQ.queue_tail	=0;													
 
	
	pthread_t	Doctor	[NumDR];		
	pthread_t	Patient	[NumPatient];
 
	
	pthread_mutex_lock	(&DRsOffice);									
	int	count	=0;
	for (count=0; count < NumDR; ++count){								
		pthread_create ( &Doctor[count], NULL, doctor_thread, NULL);	
		printf ("@user: Doctor Thread #%d is created", count);
	}
	pthread_mutex_unlock (&DRsOffice);									
 

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
