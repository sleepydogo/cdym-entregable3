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
#include <avr/pgmspace.h>

void sound(unsigned int freq, unsigned int dur);
void RTTTL_play_song(void);
void RTTTL_play_note(void);
void RTTTL_flags_interrupcion(void);
void RTTTL_cambiar_cancion(uint8_t cancion_elegida);
void RTTTL_stop_song(void);
uint8_t RTTTL_Flag_stop_status();

#endif /* RTTTL_H_ */