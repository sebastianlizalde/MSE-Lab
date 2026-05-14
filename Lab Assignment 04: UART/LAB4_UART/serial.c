/**
 * @file serial.c
 * @brief Serial module 
 *
 *
 * @author Melanie Picen
 * @date 2026-05-14
 */

/*** Includes ***/
#include "serial.h"
#include "UART_Driver.h"
#include "utils.h"
#include <stdarg.h>

/*** Preprocessor Definitions ***/

/*** Type Prototypes ***/

/*** Local Variables ***/

static char serial_buffer[SERIAL_BUFFER_SIZE];

/*** External Variables ***/

/*** Function Prototypes ***/

/*** Function Definitions ***/

void serial_init(void)
{
    (void)uart_init(UART_SYSTEM_CLOCK_HZ, UART_DEFAULT_BAUDRATE);
}


void serial_printf(const char *format, ...)
{
    va_list     args;
    const char *fmt = format;
    int         pos = 0;
    uint32_t    i   = 0U;

    va_start(args, format);

    while ((*fmt != '\0') && (pos < (int)(SERIAL_BUFFER_SIZE - 1U)))
    {
        if (*fmt == '%')
        {
            fmt++;

            switch (*fmt)
            {
                case 'd':
                {
                    int32_t  val = va_arg(args, int32_t);
                    uint8_t  tmp[24];
                    uint32_t len = utils_itoa(val, tmp, 1U, 10U);
                    uint32_t k;

                    for (k = 0U; (k < len) && (pos < (int)(SERIAL_BUFFER_SIZE - 1U)); k++)
                    {
                        serial_buffer[pos] = (char)tmp[k];
                        pos++;
                    }
                    break;
                }
                case 'u':
                {
                    uint32_t val = va_arg(args, uint32_t);
                    uint8_t  tmp[24];
                    uint32_t len = utils_itoa((int32_t)val, tmp, 0U, 10U);
                    uint32_t k;

                    for (k = 0U; (k < len) && (pos < (int)(SERIAL_BUFFER_SIZE - 1U)); k++)
                    {
                        serial_buffer[pos] = (char)tmp[k];
                        pos++;
                    }
                    break;
                }
                case 'x':
                {
                    uint32_t val = va_arg(args, uint32_t);
                    uint8_t  tmp[24];
                    uint32_t len = utils_itoa((int32_t)val, tmp, 0U, 16U);
                    uint32_t k;

                    for (k = 0U; (k < len) && (pos < (int)(SERIAL_BUFFER_SIZE - 1U)); k++)
                    {
                        serial_buffer[pos] = (char)tmp[k];
                        pos++;
                    }
                    break;
                }
                case 's':
                {
                    char *s = va_arg(args, char *);

                    while ((*s != '\0') && (pos < (int)(SERIAL_BUFFER_SIZE - 1U)))
                    {
                        serial_buffer[pos] = *s;
                        pos++;
                        s++;
                    }
                    break;
                }
                case 'c':
                {
                    serial_buffer[pos] = (char)va_arg(args, int);
                    pos++;
                    break;
                }
                case '%':
                {
                    serial_buffer[pos] = '%';
                    pos++;
                    break;
                }
                default:
                {
                    serial_buffer[pos] = '%';
                    pos++;
                    if (pos < (int)(SERIAL_BUFFER_SIZE - 1U))
                    {
                        serial_buffer[pos] = *fmt;
                        pos++;
                    }
                    break;
                }
            }
        }
        else
        {
            serial_buffer[pos] = *fmt;
            pos++;
        }

        fmt++;
    }

    serial_buffer[pos] = '\0';
    va_end(args);

    while (serial_buffer[i] != '\0')
    {
        uart_write((uint8_t)serial_buffer[i]);
        i++;
    }
}