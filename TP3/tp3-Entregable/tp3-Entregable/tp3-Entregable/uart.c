/*
 * uart.c
 *
 * Created: 09/06/2023 18:52:36
 *  Author: sleepydogo
 */

#include "uart.h"

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

// Inicializamos el driver
void uart_init(struct uart_driver *driver, char *tx_buffer, int tx_size, char *rx_buffer, int rx_size, void (*tx_interrupt_handler)(void), void (*rx_interrupt_handler)(void), uint8_t baud_rate)
{
    driver->tx_buffer = tx_buffer;
    driver->rx_buffer = rx_buffer;
    driver->tx_size = tx_size;
    driver->rx_size = rx_size;
    driver->tx_interrupt_handler = tx_interrupt_handler;
    driver->rx_interrupt_handler = rx_interrupt_handler;

    // Seteamos el baud rate de acuerdo a baud_rate
    UBRR0H = (unsigned char)(baud_rate >> 8);
    UBRR0L = (unsigned char)baud_rate;

    // Deshabilitamos todas las funciones del USART
    UCSR0B = 0;

    // Seteamos el data type del UART
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}


// Transmision de datos
void uart_transmit(struct uart_driver *driver, char *data, int size)
{
    // Checkeamos si hay suficiente espacio en el buffer de transmision
    if (size > driver->tx_size)
    {
        // No hay suficiente espacio..
        return;
    }

    // Copiamos los datos al buffer de transmision
    memcpy(driver->tx_buffer, data, size);

    // Habilitamos la interrupcion de transmision
    UCSR0B |= (1 << UDRIE0);
}

// Recepcion de datos
char *uart_receive(struct uart_driver *driver, int size)
{
    // Checkeamos si hay suficiente espacio en el buffer de recepcion
    if (size > driver->rx_size)
    {
        // No hay suficiente espacio..
        return NULL;
    }

    // Copiamos los datos en data desde el buffer de recepcion 
    char *data = driver->rx_buffer;

    // Deshabilitamos las interrupciones de recepcion
    UCSR0B &= ~(1 << RXCIE0);

    // Retornamos los datos...
    return data;
}

// Handler de interrupciones de transmision
ISR(USART0_UDRE_vect)
{
    // Transmitimos los byte al buffer de transmision uno a uno
    UDR0 = *uart_driver->tx_buffer++;

    // Si el buffer de transmision esta vacio, deshabilitamos las interrupciones de transmision 
    if (uart_driver->tx_buffer == uart_driver->tx_end)
    {
        UCSR0B &= ~(1 << UDRIE0);
    }
}

// Handler de interrupciones de recepcion
ISR(USART0_RX_vect)
{
    // Leemos los bytes del buffer de recepcion uno a uno
    *uart_driver->rx_buffer++ = UDR0;

    // If the receive buffer is full, signal the consumer thread
    // Si el buffer esta lleno, señalamos al thread de consumidor
    if (uart_driver->rx_buffer == uart_driver->rx_end)
    {
        uart_driver->rx_interrupt_handler();
    }
}