#ifndef __DOCTOR_PATIENT_H__
#define __DOCTOR_PATIENT_H__


#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>


void Alert_the_Doctor();
int Check_the_Vitals( int Breathing_rate, int Heart_beat);
void sighup() ;
void sigint();
void sigquit();
void* pat1(void* pv);
void* pat2(void* pv);
void* pat3(void* pv);




#endif
