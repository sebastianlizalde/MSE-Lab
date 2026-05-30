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
 * @brief Utility library with helper functions.
 *
 * Utils module has helper functions to treat strings, ASCII conversions, and
 * printing utilities.
 *
 * @author Melanie, Michelle, Sebastian
 * @date 04/29/2026
 *
 */

#ifndef __UTILS_H__
#define __UTILS_H__

/*** Includes ***/
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

/*** Preprocessor Definitions ***/

/*** Type Prototypes ***/

typedef enum{
    BINARY = 0u,
    OCTAL = 1u,
    DECIMAL = 2u,
    HEX = 3u,
    TOTAL = 4u
} num_base;


typedef enum{
    UNSIGNED = 0u,
    SIGNED = 1u,
} num_sign;


/*** Global Variables ***/

/*** Function Prototypes ***/

void utils_snprintf(char *dst, const char *format, ...);

void utils_vsnprintf(char *dst, const char *format, va_list args);


uint32_t utils_itoa(int32_t data, uint8_t *ptr, uint8_t sign, uint8_t base);


int32_t utils_atoi(uint8_t *ptr, uint32_t digits, uint8_t sign, uint8_t base);


void * utils_memCpy(void *dst, void *src, size_t length);

void * utils_memReverse(void *src, size_t length);

#endif /* __UTILS_H__ */
