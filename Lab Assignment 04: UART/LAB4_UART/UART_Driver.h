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
* @file UART_Driver.h
* @brief UART driver interface for USART2 on STM32F411RE.
*
* This module provides a low-level UART driver for the STM32F411RE
* microcontroller. It exposes an API to initialize the USART2 peripheral,
* configure the baud rate based on the system clock, and transmit a single
* byte of data. All operations act directly on hardware registers without
* HAL dependency.
*
* Hardware mapping (NUCLEO-F411RE):
*   - USART2_TX : PA2 (Alternate Function 7)
*   - USART2_RX : PA3 (Alternate Function 7)
*   - Connected to ST-LINK virtual COM port via SB13/SB14 (UM1724 sec 7.10)
*
* @author Sebastian Lizalde Herrera
* @date 2026-05-12
*
*/

#ifndef __UART_DRIVER_H__
#define __UART_DRIVER_H__

/*** Includes ***/
#include <stdint.h>

/*** Preprocessor Definitions ***/

#define USART2_BASE             (0x40004400UL) 
#define RCC_FULL_BASE           (0x40023800UL) 
#define RCC_APB1ENR_USART2EN    (1U << 17)  
#define USART_SR_TXE            (1U << 7)   
#define USART_SR_TC             (1U << 6) 
#define USART_SR_RXNE           (1U << 5)   
#define USART_CR1_UE            (1U << 13)  
#define USART_CR1_TE            (1U << 3)  
#define USART_CR1_RE            (1U << 2)  
#define UART_SYSTEM_CLOCK_HZ    (16000000UL) 
#define UART_DEFAULT_BAUDRATE   (115200UL)   

/*** Type Prototypes ***/

typedef struct
{
    volatile uint32_t SR;   
    volatile uint32_t DR;   
    volatile uint32_t BRR;  
    volatile uint32_t CR1;  
    volatile uint32_t CR2;  
    volatile uint32_t CR3;  
    volatile uint32_t GTPR; 
} USART_RegDef_t;

typedef struct
{
    volatile uint32_t CR;         
    volatile uint32_t PLLCFGR;      
    volatile uint32_t CFGR;        
    volatile uint32_t CIR;          
    volatile uint32_t AHB1RSTR;    
    volatile uint32_t AHB2RSTR;     
    volatile uint32_t RESERVED0[2]; 
    volatile uint32_t APB1RSTR;     
    volatile uint32_t APB2RSTR;    
    volatile uint32_t RESERVED1[2]; 
    volatile uint32_t AHB1ENR;      
    volatile uint32_t AHB2ENR;      
    volatile uint32_t RESERVED2[2]; 
    volatile uint32_t APB1ENR;      
    volatile uint32_t APB2ENR;      
} RCC_UART_RegDef_t;

#define USART2      ((USART_RegDef_t *)    USART2_BASE)
#define RCC_UART    ((RCC_UART_RegDef_t *) RCC_FULL_BASE)

typedef enum
{
    UART_OK             =  0,  
    UART_ERR_BAUDRATE   = -1,  
    UART_ERR_CLOCK      = -2,  
} UART_Status_t;

/*** Global Variables ***/

/*** Function Prototypes ***/

UART_Status_t uart_init(uint32_t sys_clk_hz, uint32_t baudrate);

void uart_write(uint8_t data);

#endif /* __UART_DRIVER_H__ */