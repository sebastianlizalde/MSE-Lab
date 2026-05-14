/**
 * @file serial.h
 * @brief Serial module — high-level API for terminal communication over UART.
 *
 * The Serial module provides a simple, printf-style interface on top of the
 * low-level UART driver and the Utils formatting library. It targets USART2
 * on the STM32F411RE Nucleo-64 board (PA2 TX / PA3 RX, 115200 baud).
 *
 *
 * @author Melanie Picen
 * @date 2026-05-14
 */

#ifndef __SERIAL_H__
#define __SERIAL_H__

/*** Includes ***/
#include <stdint.h>

/*** Preprocessor Definitions ***/

#define SERIAL_BUFFER_SIZE  (128U)

/*** Type Prototypes ***/

/*** Global Variables ***/

/*** Function Prototypes ***/

void serial_init(void);

void serial_printf(const char *format, ...);

#endif /* __SERIAL_H__ */