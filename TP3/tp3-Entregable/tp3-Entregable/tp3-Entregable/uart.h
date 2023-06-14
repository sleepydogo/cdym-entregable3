/*
 * uart.h
 *
 * Created: 09/06/2023 18:52:22
 *  Author: sleepydogo
 */ 

#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>

#include "seos.h"
#include "menu.h"

#define RX_BUFFER_LENGTH 256 
#define TX_BUFFER_LENGTH 256
#define ERROR_UART_FULL_BUFF -1
#define BR9600 (0x67) //0x67 = 103 -> BAUDRATE = 9600@16MHz

void UART_Init (uint8_t baud);		

void UART_TX_Interrupt_Enable(void);
void UART_TX_Interrupt_Disable(void);
void UART_RX_Interrupt_Enable(void);
void UART_RX_Interrupt_Disable(void);

void UART_Update(int * Error_code);									
void UART_Write_String_To_Buffer(const char* STR_PTR);	
void UART_Write_Char_To_Buffer (const char data, int * Error_code);		
	
#endif // UART_H
