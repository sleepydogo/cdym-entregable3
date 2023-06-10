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

// UART driver
struct uart_driver
{
	// Buffer de transmision
	char *tx_buffer;
	// Buffer de recepcion
	char *rx_buffer;
	// Tamaño de buffer de transmision
	int tx_size;
	// Tamaño de buffer de recepcion
	int rx_size;
	// Handler de interrupciones de transmision...
	void (*tx_interrupt_handler)(void);
	// Handler de interrupciones de recepcion
	void (*rx_interrupt_handler)(void);
};

void uart_init(struct uart_driver *driver, char *tx_buffer, int tx_size, char *rx_buffer, int rx_size, void (*tx_interrupt_handler)(void), void (*rx_interrupt_handler)(void), uint8_t baud_rate);

void uart_transmit(struct uart_driver *driver, char *data, int size);

char *uart_receive(struct uart_driver *driver, int size);

ISR(USART0_UDRE_vect);

ISR(USART0_RX_vect);

#endif // UART_H
