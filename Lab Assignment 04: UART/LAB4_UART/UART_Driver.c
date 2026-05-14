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
* @file UART_Driver.c
* @brief UART driver implementation for USART2 on STM32F411RE.
*
* Implements uart_init() and uart_write() by manipulating USART2 hardware
* registers directly, as specified in the UART SRS v1.0. GPIO pins PA2 (TX)
* and PA3 (RX) are configured in Alternate Function 7 mode using the existing
* GPIO driver before enabling the USART2 peripheral.
*
* @author Sebastian Lizalde Herrera
* @date 2026-05-14
*
*/

/*** Includes ***/
#include "UART_Driver.h"
#include "GPIO_stm32f4.h"

/*** Preprocessor Definitions ***/

/*** Type Prototypes ***/

/*** Local Variables ***/

/*** External Variables ***/

/*** Function Prototypes ***/

/*** Function Definitions ***/

UART_Status_t uart_init(uint32_t sys_clk_hz, uint32_t baudrate)
{
    if (sys_clk_hz == 0U)
    {
        return UART_ERR_CLOCK;
    }

    if (baudrate == 0U)
    {
        return UART_ERR_BAUDRATE;
    }

    gpio_initPort(GPIO_PORT_A);
    gpio_setPinMode(GPIO_PORT_A, 2U, GPIO_MODE_ALTFN);
    gpio_setPinMode(GPIO_PORT_A, 3U, GPIO_MODE_ALTFN);
    gpio_setAlternateFunction(GPIO_PORT_A, 2U, GPIO_AF7);
    gpio_setAlternateFunction(GPIO_PORT_A, 3U, GPIO_AF7);

    RCC_UART->APB1ENR |= RCC_APB1ENR_USART2EN;

    USART2->BRR = sys_clk_hz / baudrate;

    USART2->CR1 |= (USART_CR1_TE | USART_CR1_UE);

    return UART_OK;
}

void uart_write(uint8_t data)
{
    while (!(USART2->SR & USART_SR_TXE))
    {
        
    }

    USART2->DR = (uint32_t)(data & 0xFFU);
}