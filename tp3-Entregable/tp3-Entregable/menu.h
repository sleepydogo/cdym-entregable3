/*
 * menu.h
 *
 * Created: 09/06/2023 18:58:10
 *  Author: Tomas E. Schattmann y Mariano A. Rodriguez Mesa
 */ 


#ifndef MENU_H_
#define MENU_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rtttl.h"
#include "main.h"
#include "uart.h"

#define ESTADO_PLAY 1
#define ESTADO_STOP 2
#define ESTADO_NUM 3
#define ESTADO_RESET 4

void MENU_Show_Menu(void);
void MENU_Command_Update(const char * RX_buffer);
void MENU_Perform_Task(void);
void MENU_Show_Canciones(void);
uint8_t MENU_Compare_Command(const char* str1, const char* str2, uint8_t is_num);
void MENU_Show_Option(char *option, uint8_t lenght);


#endif /* MENU_H_ */