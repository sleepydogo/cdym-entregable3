/*
 * seos.h
 *
 * Created: 09/06/2023 18:57:39
 *  Author: Tomas E. Schattmann, Mariano A. Rodriguez Mesa
 */ 


#ifndef SEOS_H_
#define SEOS_H_


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "rtttl.h" 

void SEOS_Init_Timer();
void SEOS_Schedule_Tasks();
void SEOS_Dispatch_Tasks();
void SEOS_Go_To_Sleep();

#endif /* SEOS_H_ */