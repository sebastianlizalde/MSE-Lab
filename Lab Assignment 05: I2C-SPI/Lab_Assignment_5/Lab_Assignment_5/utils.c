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
 * @brief Utility library with helper functions.
 *
 * Utils module has helper functions to treat strings, ASCII conversions, and
 * printing utilities.
 *
 * @author Melanie, Michelle, Sebastian
 * @date 05/20/2026
 *
 */

/*** Includes ***/
#include "utils.h"

/*** Preprocessor Definitions ***/
#define ASCII_SIGN          45u
#define ASCII_NUM_OFFSET    '0'
#define ASCII_CHAR_OFFSET   'A'

/*** Type Prototypes ***/

/*** Local Variables ***/

/*** External Variables ***/

/*** Function Prototypes ***/

static uint32_t utils_printString(char *dst, char *src);
static uint32_t utils_printInt(char *dst, int32_t num, uint8_t sign, uint8_t base);



/*** Function Definitions ***/

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
                case 's': {
                    char *str = va_arg(args, char*);
                    if (!str) str = "(null)";
                    dst += utils_printString(dst, str);
                    break;
                }

                case 'd': {
                    int32_t num = va_arg(args, int);
                    dst += utils_printInt(dst, num, SIGNED, DECIMAL);
                    break;
                }

                case 'u': {
                    int32_t num = (int32_t)va_arg(args, unsigned int);
                    dst += utils_printInt(dst, num, UNSIGNED, DECIMAL);
                    break;
                }

                case 'x': {
                    int32_t num = (int32_t)va_arg(args, unsigned int);
                    dst += utils_printInt(dst, num, UNSIGNED, HEX);
                    break;
                }

                case 'c':
                    *dst++ = (char)va_arg(args, int);
                    break;

                case '%':
                    *dst++ = '%';
                    break;

                default:
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



void utils_vsnprintf(char *dst, const char *format, va_list args){

    while (*format)
    {
        if (*format == '%')
        {
            format++;

            switch (*format)
            {
                case 's': {
                    char *str = va_arg(args, char*);
                    if (!str) str = "(null)";
                    dst += utils_printString(dst, str);
                    break;
                }

                case 'd': {
                    int32_t num = va_arg(args, int);
                    dst += utils_printInt(dst, num, SIGNED, DECIMAL);
                    break;
                }

                case 'u': {
                    int32_t num = (int32_t)va_arg(args, unsigned int);
                    dst += utils_printInt(dst, num, UNSIGNED, DECIMAL);
                    break;
                }

                case 'x': {
                    int32_t num = (int32_t)va_arg(args, unsigned int);
                    dst += utils_printInt(dst, num, UNSIGNED, HEX);
                    break;
                }

                case 'c':
                    *dst++ = (char)va_arg(args, int);
                    break;

                case '%':
                    *dst++ = '%';
                    break;

                default:
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

}

uint32_t utils_itoa(int32_t data, uint8_t *ptr, uint8_t sign, uint8_t base){

    uint8_t n_base;
    uint32_t curr_num;
    uint32_t len_str = 0u;
    uint32_t digit;

    switch(base){
        case BINARY:    n_base = 2u;    break;
        case OCTAL:     n_base = 8u;    break;
        case DECIMAL:   n_base = 10u;   break;
        case HEX:       n_base = 16u;   break;
        default:        return 0;
    }

    if(sign == SIGNED && data < 0){
        *(ptr + len_str) = '-';
        len_str++;
        curr_num = (uint32_t)(-(int64_t)data);
    } else {
        curr_num = (uint32_t)data;
    }

    do {
        digit = curr_num % n_base;
        curr_num /= n_base;

        if(digit < 10){
            *(ptr + len_str) = ASCII_NUM_OFFSET + digit;
        } else {
            *(ptr + len_str) = ASCII_CHAR_OFFSET + digit - 10;
        }

        len_str++;
    } while (curr_num != 0);

    uint32_t start = (sign == SIGNED && data < 0) ? 1u : 0u;
    uint32_t end = len_str - 1u;
    uint8_t temp;

    for(uint32_t i = start; i <= (start + end)/2u; i++){
        temp = *(ptr + i);
        *(ptr + i) = *(ptr + (end - (i - start)));
        *(ptr + (end - (i - start))) = temp;
    }

    *(ptr + len_str) = '\0';

    return len_str;
}


int32_t utils_atoi(uint8_t *ptr, uint32_t digits, uint8_t sign, uint8_t base){

    int32_t num = 0;
    uint32_t i = 0;
    uint8_t n_base;
    int8_t sign_flag = 1;
    uint32_t digit;

    switch(base){
        case BINARY:    n_base = 2u;    break;
        case OCTAL:     n_base = 8u;    break;
        case DECIMAL:   n_base = 10u;   break;
        case HEX:       n_base = 16u;   break;
        default:        return 0;
    }

    if(sign == SIGNED && *ptr == '-'){
        sign_flag = -1;
        ptr++;  
        digits--;
    }

    for(i = 0; i < digits && *(ptr + i) != '\0'; i++){

        uint8_t c = *(ptr + i);

        if(c >= '0' && c <= '9'){
            digit = c - '0';
        }
        else if(c >= 'A' && c <= 'F'){
            digit = c - 'A' + 10;
        }
        else if(c >= 'a' && c <= 'f'){
            digit = c - 'a' + 10;
        }
        else{
            return 0; 
        }

        if(digit >= n_base){
            return 0; 
        }

        num = num * n_base + digit;
    }

    return num * sign_flag;
}

void * utils_memCpy(void *dst, void *src, size_t length){

    /* punteros de tipo byte para poder moverse caracter por caracter */
    uint8_t *d = (uint8_t *)dst;
    uint8_t *s = (uint8_t *)src;
    size_t i;

    /* copia byte por byte desde src hacia dst */
    for(i = 0; i < length; i++)
    {
        *(d + i) = *(s + i);
    }

    return dst;
}


void * utils_memReverse(void *src, size_t length){

    uint8_t *s = (uint8_t *)src;
    uint8_t temp;
    size_t i;

    /* intercambia el primer elemento con el ultimo, luego el segundo con el penultimo, etc */
    for(i = 0; i < length / 2; i++)
    {
        temp = *(s + i);
        *(s + i) = *(s + (length - 1 - i));
        *(s + (length - 1 - i)) = temp;
    }

    return src;

}


static uint32_t utils_printString(char *dst, char *src)
{
    uint32_t len = 0;
    while (src[len]) len++;
 
    utils_memCpy(dst, src, len);
 
    return len;
}

static uint32_t utils_printInt(char *dst, int32_t num, uint8_t sign, uint8_t base)
{
    uint8_t buf[12];
 
    uint32_t len = utils_itoa(num, buf, sign, base);
 
    utils_memCpy(dst, buf, len);
 
    return len;
}
