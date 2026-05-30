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
* @file SPI_Driver.c
* @brief SPI driver implementation for SPI1 on STM32F411RE.
*
* Implements spi_init, spi_transmit, spi_receive, spi_csEnable, and
* spi_csDisable by manipulating SPI1 and GPIO hardware registers directly.
*
* Pin mapping (SPI1, AF5):
*   PA4 - CS   (GPIO output, active low, software-controlled)
*   PA5 - SCK  (SPI1_SCK)
*   PA6 - MISO (SPI1_MISO)
*   PA7 - MOSI (SPI1_MOSI)
*
* @author Melanie Picen
* @date 2026
*
*/

/*** Includes ***/
#include "SPI_Driver.h"

/*** Preprocessor Definitions ***/

/*** Type Prototypes ***/

/*** Local Variables ***/

/*** External Variables ***/

/*** Function Prototypes ***/

static SPI_Status_t spi_waitTxEmpty(void);
static SPI_Status_t spi_waitRxNotEmpty(void);
static SPI_Status_t spi_waitNotBusy(void);

/*** Function Definitions ***/

SPI_Status_t spi_init(void)
{
    volatile uint32_t *rcc_ahb1enr  = (volatile uint32_t *)(RCC_BASE_SPI  + RCC_AHB1ENR_OFFSET_SPI);
    volatile uint32_t *rcc_apb2enr  = (volatile uint32_t *)(RCC_BASE_SPI  + RCC_APB2ENR_OFFSET);

    volatile uint32_t *gpioa_moder  = (volatile uint32_t *)(GPIOA_BASE_SPI + GPIO_MODER_OFFSET_SPI);
    volatile uint32_t *gpioa_otyper = (volatile uint32_t *)(GPIOA_BASE_SPI + GPIO_OTYPER_OFFSET_SPI);
    volatile uint32_t *gpioa_ospeedr= (volatile uint32_t *)(GPIOA_BASE_SPI + GPIO_OSPEEDR_OFFSET_SPI);
    volatile uint32_t *gpioa_pupdr  = (volatile uint32_t *)(GPIOA_BASE_SPI + GPIO_PUPDR_OFFSET_SPI);
    volatile uint32_t *gpioa_bsrr   = (volatile uint32_t *)(GPIOA_BASE_SPI + GPIO_BSRR_OFFSET);
    volatile uint32_t *gpioa_afrl   = (volatile uint32_t *)(GPIOA_BASE_SPI + GPIO_AFRL_OFFSET);

    /* Enable GPIOA clock (FR-2)  */
    *rcc_ahb1enr |= RCC_AHB1ENR_GPIOAEN;

    /* Configure PA4 (CS) as push-pull output  */
    *gpioa_moder &= ~(0x3U << (4U * 2U));
    *gpioa_moder |=  (GPIO_MODER_OUT_SPI << (4U * 2U));

    /* CS output type: push-pull  */
    *gpioa_otyper &= ~(1U << 4U);

    /* CS output speed high */
    *gpioa_ospeedr &= ~(0x3U << (4U * 2U));
    *gpioa_ospeedr |=  (GPIO_OSPEED_HIGH_SPI << (4U * 2U));

    /* CS pull none */
    *gpioa_pupdr &= ~(0x3U << (4U * 2U));

    /* Deassert CS (PA4 high) before enabling SPI */
    *gpioa_bsrr = GPIO_BSRR_BS4;

    /* Configure PA5, PA6, PA7 as AF5 (SPI1)  */
    *gpioa_moder &= ~((0x3U << (5U * 2U)) |
                      (0x3U << (6U * 2U)) |
                      (0x3U << (7U * 2U)));
    *gpioa_moder |=  ((GPIO_MODER_ALTFN_SPI << (5U * 2U)) |
                      (GPIO_MODER_ALTFN_SPI << (6U * 2U)) |
                      (GPIO_MODER_ALTFN_SPI << (7U * 2U)));

    /* Push-pull output type for SCK and MOSI; MISO input by default */
    *gpioa_otyper &= ~((1U << 5U) | (1U << 7U));

    /* High speed for all three SPI pins */
    *gpioa_ospeedr &= ~((0x3U << (5U * 2U)) |
                        (0x3U << (6U * 2U)) |
                        (0x3U << (7U * 2U)));
    *gpioa_ospeedr |=  ((GPIO_OSPEED_HIGH_SPI << (5U * 2U)) |
                        (GPIO_OSPEED_HIGH_SPI << (6U * 2U)) |
                        (GPIO_OSPEED_HIGH_SPI << (7U * 2U)));

    /* No pull resistors on SPI pins */
    *gpioa_pupdr &= ~((0x3U << (5U * 2U)) |
                      (0x3U << (6U * 2U)) |
                      (0x3U << (7U * 2U)));

    /* Set AF5 for PA5, PA6, PA7 in AFRL */
    *gpioa_afrl &= ~((0xFU << (5U * 4U)) |
                     (0xFU << (6U * 4U)) |
                     (0xFU << (7U * 4U)));
    *gpioa_afrl |=  ((GPIO_AF5 << (5U * 4U)) |
                     (GPIO_AF5 << (6U * 4U)) |
                     (GPIO_AF5 << (7U * 4U)));

    /*  Enable SPI1 peripheral clock (FR-2)  */
    *rcc_apb2enr |= RCC_APB2ENR_SPI1EN;

    /* Configure SPI1 (FR-1) */
    /* Disable SPI before configuring */
    SPI1->CR1 &= ~SPI_CR1_SPE;

    /* Set master mode, baud rate fPCLK/8, mode 0 (CPOL=0, CPHA=0),
     * 8-bit frames, software slave management, SSI=1               */
    SPI1->CR1 = SPI_CR1_MSTR    |
                SPI_CR1_BR_DIV8 |
                SPI_CR1_SSM     |
                SPI_CR1_SSI;
    /* DFF=0 (8-bit), CPOL=0, CPHA=0 are the reset values           */

    /* Enable SPI1 */
    SPI1->CR1 |= SPI_CR1_SPE;

    return SPI_OK;
}

SPI_Status_t spi_transmit(const uint8_t *data, uint32_t length)
{
    SPI_Status_t status;
    uint32_t     i;
    uint8_t      dummy;

    if ((data == (void *)0) || (length == 0U))
    {
        return SPI_ERR_NULL;
    }

    for (i = 0U; i < length; i++)
    {
        /* Wait until TX buffer is empty (FR-4) */
        status = spi_waitTxEmpty();
        if (status != SPI_OK)
        {
            return status;
        }

        SPI1->DR = (uint32_t)data[i];

        /* Wait for RX buffer to fill so we can clear it */
        status = spi_waitRxNotEmpty();
        if (status != SPI_OK)
        {
            return status;
        }

        /* Read and discard the received byte to prevent overrun */
        dummy = (uint8_t)SPI1->DR;
        (void)dummy;
    }

    /* Wait until the bus is no longer busy (FR-4) */
    status = spi_waitNotBusy();
    return status;
}

SPI_Status_t spi_receive(uint8_t *data, uint32_t length)
{
    SPI_Status_t status;
    uint32_t     i;

    if ((data == (void *)0) || (length == 0U))
    {
        return SPI_ERR_NULL;
    }

    for (i = 0U; i < length; i++)
    {
        /* Send dummy byte to generate clock pulses (FR-6) */
        status = spi_waitTxEmpty();
        if (status != SPI_OK)
        {
            return status;
        }

        SPI1->DR = (uint32_t)SPI_DUMMY_BYTE;

        /* Wait for received byte (FR-5) */
        status = spi_waitRxNotEmpty();
        if (status != SPI_OK)
        {
            return status;
        }

        data[i] = (uint8_t)SPI1->DR;
    }

    /* Wait until the bus is no longer busy (FR-6) */
    status = spi_waitNotBusy();
    return status;
}

void spi_csEnable(void)
{
    volatile uint32_t *gpioa_bsrr = (volatile uint32_t *)(GPIOA_BASE_SPI + GPIO_BSRR_OFFSET);
    *gpioa_bsrr = GPIO_BSRR_BR4;
}

void spi_csDisable(void)
{
    volatile uint32_t *gpioa_bsrr = (volatile uint32_t *)(GPIOA_BASE_SPI + GPIO_BSRR_OFFSET);
    *gpioa_bsrr = GPIO_BSRR_BS4;
}

/*** Static Helper Functions ***/

static SPI_Status_t spi_waitTxEmpty(void)
{
    uint32_t timeout = SPI_TIMEOUT;

    while (!(SPI1->SR & SPI_SR_TXE))
    {
        if (SPI1->SR & SPI_SR_MODF)
        {
            return SPI_ERR_BUS;
        }

        if (--timeout == 0U)
        {
            return SPI_ERR_TIMEOUT;
        }
    }

    return SPI_OK;
}

static SPI_Status_t spi_waitRxNotEmpty(void)
{
    uint32_t timeout = SPI_TIMEOUT;

    while (!(SPI1->SR & SPI_SR_RXNE))
    {
        if (SPI1->SR & (SPI_SR_OVR | SPI_SR_MODF))
        {
            return SPI_ERR_BUS;
        }

        if (--timeout == 0U)
        {
            return SPI_ERR_TIMEOUT;
        }
    }

    return SPI_OK;
}

static SPI_Status_t spi_waitNotBusy(void)
{
    uint32_t timeout = SPI_TIMEOUT;

    while (SPI1->SR & SPI_SR_BSY)
    {
        if (--timeout == 0U)
        {
            return SPI_ERR_TIMEOUT;
        }
    }

    return SPI_OK;
}