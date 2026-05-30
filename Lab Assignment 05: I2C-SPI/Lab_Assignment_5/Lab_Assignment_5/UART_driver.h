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
 * @file    uart_driver.h
 * @brief   UART Driver – Software Requirements Specification v1.0
 * @author  Melanie, Michelle, Sebastian
 * 
 * @details Provides a portable, register-level UART abstraction for the
 *          STM32F411RE microcontroller (RM0383). Targets the USART2 peripheral
 *          for asynchronous serial transmission. All public functions return a
 *          uart_status_t code so that callers can perform error handling
 *          (NFR-1, NFR-2).
 */

#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include <stdint.h>
#include <stddef.h>

#ifndef __I
#define __I     volatile const   /**< Read-only  */
#endif
#ifndef __O
#define __O     volatile         /**< Write-only */
#endif
#ifndef __IO
#define __IO    volatile         /**< Read/Write */
#endif

#ifndef RCC_BASE
#define RCC_BASE        0x40023800UL
#endif

/** RCC APB1 peripheral clock enable register (RM0383 §6.3.12) */
#ifndef RCC_APB1ENR
#define RCC_APB1ENR     (*((__IO uint32_t *)(RCC_BASE + 0x40UL)))
#endif
    
/** Bit position of the USART2 clock-enable flag inside RCC_APB1ENR */
#define RCC_APB1ENR_USART2EN_POS    (17U)

typedef struct
{
    __IO uint32_t SR;    /**< 0x00 – Status register              */
    __IO uint32_t DR;    /**< 0x04 – Data register                */
    __IO uint32_t BRR;   /**< 0x08 – Baud rate register           */
    __IO uint32_t CR1;   /**< 0x0C – Control register 1           */
    __IO uint32_t CR2;   /**< 0x10 – Control register 2           */
    __IO uint32_t CR3;   /**< 0x14 – Control register 3           */
    __IO uint32_t GTPR;  /**< 0x18 – Guard time and prescaler reg */
} USART_RegDef_t;

/** Convenience pointer – dereference to access USART2 registers directly */
#define USART2  ((USART_RegDef_t *) 0x40004400UL)

/** TXE: Transmit data register empty (FR-4) */
#define USART_SR_TXE_POS    (7U)

/** TC: Transmission complete */
#define USART_SR_TC_POS     (6U)

/** RXNE: Read data register not empty */
#define USART_SR_RXNE_POS   (5U)

/** UE: USART enable (FR-3) */
#define USART_CR1_UE_POS    (13U)

/** TE: Transmitter enable (FR-3) */
#define USART_CR1_TE_POS    (3U)

/** RE: Receiver enable */
#define USART_CR1_RE_POS    (2U)

/** APB1 peripheral clock frequency in Hz (default: 16 MHz HSI) */
#ifndef UART_PCLK_HZ
#define UART_PCLK_HZ        16000000UL
#endif

/** Target baud rate (default: 115 200 bps – FR-2, Task 4) */
#ifndef UART_BAUD_RATE
#define UART_BAUD_RATE      115200UL
#endif

#define UART_BRR_VALUE      (UART_PCLK_HZ / UART_BAUD_RATE)

/**
 * @brief Return codes for all UART driver functions (NFR-1, NFR-2).
 */
typedef enum
{
    UART_OK                  = 0U,  /**< Operation succeeded                  */
    UART_ERR_INVALID_BAUD    = 1U,  /**< Baud-rate argument is zero or invalid*/
    UART_ERR_NULL_POINTER    = 2U,  /**< A required pointer was NULL          */
    UART_ERR_NOT_INITIALISED = 3U,  /**< uart_write called before uart_init() */
    UART_ERR_TIMEOUT         = 4U   /**< TXE flag did not clear in time       */
} uart_status_t;


/**
 * @brief   Initialises the USART2 peripheral (FR-1, FR-2, FR-3).
 *
 * @details Performs the following sequence:
 *            1. Enables the USART2 peripheral clock via RCC_APB1ENR  (FR-1).
 *            2. Writes the pre-computed baud-rate divisor to BRR      (FR-2).
 *            3. Sets TE (Transmitter Enable) and UE (USART Enable)
 *               in CR1 to activate the peripheral                     (FR-3).
 *
 *          The GPIO pins (PA2 – TX, PA3 – RX) must be configured for
 *          Alternate Function 7 (AF7) by the caller via gpio_driver before
 *          invoking this function.
 *
 * @return  UART_OK on success, or a uart_status_t error code.
 *
 * @note    Example usage:
 * @code
 *          // 1. Configure PA2 as AF7 (USART2_TX) using gpio_driver
 *          gpio_initPort(GPIO_PORT_A);
 *          gpio_setPinMode(GPIO_PORT_A, GPIO_PIN_2, GPIO_MODE_ALT_FUNC);
 *          gpio_setAlternateFunction(GPIO_PORT_A, GPIO_PIN_2, GPIO_AF7);
 *
 *          // 2. Initialise UART at 115 200 bps
 *          uart_status_t status = uart_init();
 * @endcode
 */
uart_status_t uart_init(void);

/**
 * @brief   Transmits a single byte over USART2 (FR-4, FR-5).
 *
 * @details Performs the following sequence:
 *            1. Polls the TXE flag in the SR register until the transmit
 *               data register is empty                                 (FR-4).
 *            2. Writes the 8-bit @p data value to DR, triggering
 *               hardware transmission                                  (FR-5).
 *
 * @param   data  The byte to transmit.
 * @return  UART_OK on success, UART_ERR_NOT_INITIALISED if uart_init() has
 *          not been called, or UART_ERR_TIMEOUT if TXE never asserted.
 *
 * @note    Example usage:
 * @code
 *          uart_write('A');           // sends ASCII character 'A'
 *          uart_write(0x0A);          // sends newline (LF)
 * @endcode
 */
uart_status_t uart_write(uint8_t data);

#endif /* UART_DRIVER_H */
