/*
 * rtttl.h
 *
 * Created: 09/06/2023 18:54:35
 *  Author: Usuario
 */ 


#ifndef RTTTL_H_
#define RTTTL_H_


#include <avr/io.h>
#include <avr/interrupt.h>

void sound(unsigned int freq, unsigned int dur);
void play_song(char *song);

#endif /* RTTTL_H_ */