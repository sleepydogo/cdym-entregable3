/*
 * menu.h
 *
 * Created: 09/06/2023 18:58:10
 *  Author: Usuario
 */ 


#ifndef MENU_H_
#define MENU_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rtttl.h"
#include "main.h"
#include "uart.h"

#define MENU_OPTIONS_CANT 5

#define ESTADO_PLAY 1
#define ESTADO_STOP 2
#define ESTADO_NUM 3
#define ESTADO_RESET 4

void MENU_Show_Menu(void);
void MENU_Command_Update(const char * RX_buffer);
void MENU_Perform_Task(void);
void MENU_Show_Canciones(void);
uint8_t MENU_compareCommand(const char* str1, const char* str2, uint8_t is_num);
void MENU_Show_Option(char *option, uint8_t lenght);


#endif /* MENU_H_ */