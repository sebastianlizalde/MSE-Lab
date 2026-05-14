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
* @file utils.c
* @brief Utility module implementation — string formatting and data conversion.
*
* Implements utils_itoa() for integer-to-ASCII conversion and
* utils_snprintf() for printf-style string formatting. Both functions
* operate without any standard library dependency, making them suitable
* for bare-metal embedded use on the STM32F411RE.
*
* @author Michelle Urbina
* @date 2026-05-14
*/

/*** Includes ***/
#include "utils.h"

/*** Preprocessor Definitions ***/

/*** Local Variables ***/

/*** Function Prototypes ***/
static uint32_t utils_printString(char *dst, char *src);
static uint32_t utils_printInt(char *dst, int32_t num, uint8_t sign, uint32_t base);

/*** Function Definitions ***/

uint32_t utils_itoa(int32_t value, uint8_t *buf, uint8_t sign, uint32_t base)
{
    uint8_t  tmp[24];
    uint32_t len    = 0U;
    uint32_t i      = 0U;
    uint32_t uval;
    uint8_t  neg    = 0U;

    if (buf == ((void *)0))
    {
        return 0U;
    }

    if ((sign != 0U) && (value < 0))
    {
        neg  = 1U;
        uval = (uint32_t)(-(value));
    }
    else
    {
        uval = (uint32_t)value;
    }

    if (uval == 0U)
    {
        buf[0] = (uint8_t)'0';
        return 1U;
    }

    while (uval > 0U)
    {
        uint32_t digit = uval % base;
        if (digit < 10U)
        {
            tmp[len] = (uint8_t)('0' + digit);
        }
        else
        {
            tmp[len] = (uint8_t)('a' + (digit - 10U));
        }
        len++;
        uval /= base;
    }

    if (neg != 0U)
    {
        tmp[len] = (uint8_t)'-';
        len++;
    }

    for (i = 0U; i < len; i++)
    {
        buf[i] = tmp[len - 1U - i];
    }

    return len;
}

static uint32_t utils_printString(char *dst, char *src)
{
    uint32_t count = 0U;

    if ((dst == ((void *)0)) || (src == ((void *)0)))
    {
        return 0U;
    }

    while (*src != '\0')
    {
        *dst = *src;
        dst++;
        src++;
        count++;
    }

    return count;
}

static uint32_t utils_printInt(char *dst, int32_t num, uint8_t sign, uint32_t base)
{
    uint8_t  tmp[24];
    uint32_t len = 0U;
    uint32_t i   = 0U;

    if (dst == ((void *)0))
    {
        return 0U;
    }

    len = utils_itoa(num, tmp, sign, base);

    for (i = 0U; i < len; i++)
    {
        dst[i] = (char)tmp[i];
    }

    return len;
}

void utils_snprintf(char *dst, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    while (*format)
    {
        if (*format == '%')
        {
            format++;
            switch (*format)
            {
                case 's':
                    dst += utils_printString(dst, va_arg(args, char *));
                    break;

                case 'd':
                    dst += utils_printInt(dst, va_arg(args, int32_t), 1U, 10U);
                    break;

                case 'u':
                    dst += utils_printInt(dst, (int32_t)va_arg(args, uint32_t), 0U, 10U);
                    break;

                case 'x':
                    dst += utils_printInt(dst, (int32_t)va_arg(args, uint32_t), 0U, 16U);
                    break;

                case 'c':
                    *dst++ = (char)va_arg(args, int);
                    break;

                case '%':
                    *dst++ = '%';
                    break;

                default:
                    *dst++ = '%';
                    *dst++ = *format;
                    break;
            }
        }
        else
        {
            *dst++ = *format;
        }

        format++;
    }

    *dst = '\0';
    va_end(args);
}