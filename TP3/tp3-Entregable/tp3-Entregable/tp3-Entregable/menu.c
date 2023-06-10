/*
 * menu.c
 *
 * Created: 09/06/2023 18:58:00
 *  Author: sleepydogo
 */

#include "menu.h"

void MENU_Show_Menu(void)
{
    // El menú se escribe en el buffer de transmisión
    UART_Write_String_To_Buffer("Menu:\n\r");
    UART_Write_String_To_Buffer("a – Read PORT1\n\r");
    UART_Write_String_To_Buffer("b – Read PORT2\n\r");
    UART_Write_String_To_Buffer("? : ");
}

void MENU_Command_Update(void)
{
    char ch;
    // Check for user inputs
    if (UART_Get_Char_From_Buffer(&Ch) != 0)
    {
        MENU_Perform_Task(Ch);
        MENU_Show_Menu();
    }
}

void MENU_Perform_Task(char c)
{
    // Echo the menu option
    UART_Write_Char_To_Buffer(c);
    UART_Write_Char_To_Buffer( ’\n\r’);
    switch (c)
    {
    case 'a':
    case 'A':
    {
        Function_1(); // Perform the task PORT1
        break;
    }
    case 'b':
    case 'B':
    {
        Function_2(); // Perform the task PORT2
        break;
    }
    }
}