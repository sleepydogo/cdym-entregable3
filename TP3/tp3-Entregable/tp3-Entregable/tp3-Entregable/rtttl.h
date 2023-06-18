/*
 * rtttl.h
 *
 * Created: 09/06/2023 18:54:35
 *  Author: Tomas E. Schattmann y Mariano A. Rodriguez Mesa
 */ 


#ifndef RTTTL_H_
#define RTTTL_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

char * RTTTL_get_song_name(uint8_t cancion_elegida);
void RTTTL_sound(unsigned int freq, unsigned int dur);
void RTTTL_play_song(uint8_t cancion_elegida);
void RTTTL_set_flag_stop(uint8_t val);
void RTTTL_interruption_handler(void);

#endif /* RTTTL_H_ */