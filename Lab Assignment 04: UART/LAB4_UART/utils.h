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
* @file utils.h
* @brief Utility module — string formatting and data conversion functions.
*
* This module provides lightweight string formatting and integer-to-ASCII
* conversion utilities for use in embedded systems without standard library
* dependencies. It is designed to support the Serial module for terminal
* communication over UART on the STM32F411RE Nucleo-64.
*
* @author Michelle Urbina
* @date 2026-05-14
*/

#ifndef __UTILS_H__
#define __UTILS_H__

/*** Includes ***/
#include <stdint.h>
#include <stdarg.h>

/*** Preprocessor Definitions ***/

#define UTILS_BUFFER_SIZE   (128U)

/*** Function Prototypes ***/

uint32_t utils_itoa(int32_t value, uint8_t *buf, uint8_t sign, uint32_t base);

void utils_snprintf(char *dst, const char *format, ...);

#endif /* __UTILS_H__ */