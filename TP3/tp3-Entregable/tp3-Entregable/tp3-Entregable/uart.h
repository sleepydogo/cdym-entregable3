/*
 * uart.h
 *
 * Created: 09/06/2023 18:52:22
 *  Author: sleepydogo
 */ 

#ifndef UART_H
#define UART_H

#include <avr/io.h>

struct uart_driver {
  char *tx_buffer;
  char *rx_buffer;
  int tx_size;
  int rx_size;
  void (*tx_interrupt_handler)(void);
  void (*rx_interrupt_handler)(void);
};

void uart_init(struct uart_driver *driver, char *tx_buffer, int tx_size, char *rx_buffer, int rx_size, void (*tx_interrupt_handler)(void), void (*rx_interrupt_handler)(void));

void uart_transmit(struct uart_driver *driver, char *data, int size);

char *uart_receive(struct uart_driver *driver, int size);

ISR(USART0_UDRE_vect);

ISR(USART0_RX_vect);

#endif // UART_H
