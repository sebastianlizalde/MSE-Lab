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
 * @file    serial.h
 * @brief   Serial Module – High-level API for terminal communication v1.0
 * @author  Melanie, Michelle y Sebastian
 *
 */

#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <stdarg.h>      /* va_list – required for serial_printf */
#include "uart_driver.h"

#define SERIAL_TX_BUFFER_SIZE   (256U)


void serial_init(void);
void serial_printf(const char *fmt, ...);

#endif /* SERIAL_H */
