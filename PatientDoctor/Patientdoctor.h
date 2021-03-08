#ifndef __PATIENTDOCTOR_H__
#define __PATIENTDOCTOR_H__

#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

void treat_patient(void);
void *doctor_thread(void *arg);
void *patient_thread(void *arg);


#endif
