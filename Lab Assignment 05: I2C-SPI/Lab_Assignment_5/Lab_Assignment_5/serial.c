/******************************************************************************
* Copyright (C) 2026 by Carlos Villarreal - CETYS Universidad
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Carlos Villarreal and CETYS Universidad are not liable for any
* misuse of this material.
*
*****************************************************************************/
/**
* @file serial.c
* @brief 
*
*
* @author Team 6
* @date 23/04/2026
*
*/

/*** Includes ***/



#include "serial.h"
#include "utils.h"
#include "UART_driver.h"


void serial_init(void){

    uart_init();

}

void serial_printf(const char *format, ...){

    char buffer[SERIAL_TX_BUFFER_SIZE];
    va_list args;
    va_start(args, format);

    utils_vsnprintf(buffer, format, args);

    for (size_t i = 0; buffer[i] != '\0'; i++) {
        uart_write((uint8_t)buffer[i]);
    }

    va_end(args);
}