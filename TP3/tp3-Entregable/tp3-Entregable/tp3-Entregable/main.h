/*
 * main.h
 *
 * Created: 09/06/2023 16:14:22
 *  Author: Usuario
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

void System_reset(void);


#endif /* MAIN_H_ */