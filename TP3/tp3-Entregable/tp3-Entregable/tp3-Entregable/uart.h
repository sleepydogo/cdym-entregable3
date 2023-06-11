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

#define FLAG_datos_recibidos 0
#define RX_BUFFER_LENGTH 100 
#define TX_BUFFER_LENGTH 100
#define ERROR_UART_FULL_BUFF -1
#define BR9600 (0x67) //0x67 = 103 -> BAUDRATE = 9600@16MHz

void UART_Update(int * Error_code);									// listo
void UART_Write_String_To_Buffer(const char* STR_PTR);	// listo
void UART_Write_Char_To_Buffer (const char data, int * Error_code);		// listo
char UART_Get_Char_From_Buffer(char *ch);				// listo

// Estas son las funciones que encapsulan el Hardware
void UART_Init (uint8_t baud);								// listo
void UART_Send_Char (char dato);						// listo
char UART_Receive_data (char *dato); 					// listo

#endif // UART_H
