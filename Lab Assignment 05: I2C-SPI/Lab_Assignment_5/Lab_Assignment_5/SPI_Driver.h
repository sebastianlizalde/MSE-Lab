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
* @file SPI_Driver.h
* @brief SPI driver interface for SPI1 on STM32F411RE.
*
* This module provides a self-contained, low-level SPI driver for the
* STM32F411RE microcontroller. It exposes an API to initialize the SPI1
* peripheral and perform data transmission, reception, and chip select
* control for external devices.
*
* Pin mapping (SPI1, AF5):
*   PA4 -> CS   (GPIO output, active low)
*   PA5 -> SCK  (SPI1_SCK)
*   PA6 -> MISO (SPI1_MISO)
*   PA7 -> MOSI (SPI1_MOSI)
*
* @author  Melanie Picen
* @date 2026 
*
*/
 
#ifndef __SPI_DRIVER_H__
#define __SPI_DRIVER_H__
 
/*** Includes ***/
#include <stdint.h>
 
/*** Preprocessor Definitions ***/
 
/* SPI peripheral base addresses */
#define SPI1_BASE_ADDR          (0x40013000UL)
#define SPI2_BASE_ADDR          (0x40003800UL)
#define SPI3_BASE_ADDR          (0x40003C00UL)
 
/* GPIO and RCC base addresses */
#define GPIOA_BASE_SPI          (0x40020000UL)
#define RCC_BASE_SPI            (0x40023800UL)
 
/* RCC register offsets */
#define RCC_AHB1ENR_OFFSET_SPI  (0x30UL)
#define RCC_APB2ENR_OFFSET      (0x44UL)
 
/* RCC enable bits */
#define RCC_AHB1ENR_GPIOAEN     (1U << 0)
#define RCC_APB2ENR_SPI1EN      (1U << 12)
 
/* GPIO register offsets */
#define GPIO_MODER_OFFSET_SPI   (0x00UL)
#define GPIO_OTYPER_OFFSET_SPI  (0x04UL)
#define GPIO_OSPEEDR_OFFSET_SPI (0x08UL)
#define GPIO_PUPDR_OFFSET_SPI   (0x0CUL)
#define GPIO_BSRR_OFFSET        (0x18UL)
#define GPIO_AFRL_OFFSET        (0x20UL)
 
/* GPIO mode values */
#define GPIO_MODER_OUT_SPI      (0x1U)  /* General-purpose output (CS pin) */
#define GPIO_MODER_ALTFN_SPI    (0x2U)  /* Alternate function (SCK, MISO, MOSI) */
#define GPIO_OSPEED_HIGH_SPI    (0x2U)  /* High speed */
#define GPIO_PUPDR_NOPULL_SPI   (0x0U)  /* No pull-up/pull-down */
#define GPIO_AF5                (0x5U)  /* AF5 = SPI1 */
 
/* CS pin (PA4) BSRR bit positions */
#define GPIO_BSRR_BS4           (1U << 4)   /* Set PA4 high  (CS deassert) */
#define GPIO_BSRR_BR4           (1U << 20)  /* Reset PA4 low (CS assert)   */
 
/* SPI CR1 register bits */
#define SPI_CR1_CPHA            (1U << 0)   /* Clock phase */
#define SPI_CR1_CPOL            (1U << 1)   /* Clock polarity */
#define SPI_CR1_MSTR            (1U << 2)   /* Master selection */
#define SPI_CR1_BR_DIV8         (0x2U << 3) /* Baud rate fPCLK/8 (~2 MHz @ 16 MHz) */
#define SPI_CR1_SPE             (1U << 6)   /* SPI enable */
#define SPI_CR1_SSI             (1U << 8)   /* Internal slave select */
#define SPI_CR1_SSM             (1U << 9)   /* Software slave management */
#define SPI_CR1_DFF             (1U << 11)  /* Data frame format (0 = 8-bit) */
 
/* SPI SR register bits */
#define SPI_SR_RXNE             (1U << 0)   /* Receive buffer not empty */
#define SPI_SR_TXE              (1U << 1)   /* Transmit buffer empty */
#define SPI_SR_BSY              (1U << 7)   /* Busy flag */
#define SPI_SR_OVR              (1U << 6)   /* Overrun flag */
#define SPI_SR_MODF             (1U << 5)   /* Mode fault */
 
/* Driver configuration */
#define SPI_TIMEOUT             (10000U)    /* Polling timeout counter */
#define SPI_DUMMY_BYTE          (0xFFU)     /* Dummy byte sent during receive */
 
/*** Type Definitions ***/
 
/**
 * @brief SPI peripheral register map.
 */
typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t CRCPR;
    volatile uint32_t RXCRCR;
    volatile uint32_t TXCRCR;
    volatile uint32_t I2SCFGR;
    volatile uint32_t I2SPR;
} SPI_RegDef_t;
 
/* Peripheral access macros */
#define SPI1    ((SPI_RegDef_t *) SPI1_BASE_ADDR)
#define SPI2    ((SPI_RegDef_t *) SPI2_BASE_ADDR)
#define SPI3    ((SPI_RegDef_t *) SPI3_BASE_ADDR)
 
/**
 * @brief SPI driver status codes.
 */
typedef enum
{
    SPI_OK          =  0,
    SPI_ERR_NULL    = -1,
    SPI_ERR_BUS     = -2,
    SPI_ERR_TIMEOUT = -3,
} SPI_Status_t;
 
/*** Function Prototypes ***/
 
SPI_Status_t spi_init(void);
 
SPI_Status_t spi_transmit(const uint8_t *data, uint32_t length);
 
SPI_Status_t spi_receive(uint8_t *data, uint32_t length);
 
void spi_csEnable(void);
 
void spi_csDisable(void);
 
#endif /* __SPI_DRIVER_H__ */