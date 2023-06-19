/*
 * main.h
 *
 * Created: 09/06/2023 16:14:22
 *  Author: Tomas E. Schattmann, Mariano A. Rodriguez Mesa
 */ 


#ifndef MAIN_H_
#define MAIN_H_
#define F_CPU 16000000UL 

#include "uart.h" 
#include "menu.h"
#include "seos.h"

#include <util/delay.h> 
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>

// Esta funcion realiza un reset del sistema despues de 15 milisegundos.
void System_reset(void);


#endif /* MAIN_H_ */