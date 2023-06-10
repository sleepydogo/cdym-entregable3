/*
 * seos.h
 *
 * Created: 09/06/2023 18:57:39
 *  Author: Usuario
 */ 


#ifndef SEOS_H_
#define SEOS_H_


#include <avr/io.h>
#include <avr/interrupt.h>

void SEOS_Schedule_Tasks();
void SEOS_Dispatch_Tasks();

#endif /* SEOS_H_ */