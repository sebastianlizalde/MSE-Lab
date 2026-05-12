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
* @file ADC_Driver.h
* @brief ADC driver interface for STM32F411RE.
*
* This module provides a low-level ADC driver for the STM32F411RE
* microcontroller. It exposes an API to initialize the ADC subsystem,
* enable ADC instances, configure regular and injected channels, start
* single, continuous, or injected conversions, and read conversion results.
* All operations act directly on hardware registers without HAL dependency.
*
* @author Sebastian Lizalde Herrera
* @date 2026-04-29
*/

#ifndef __ADC_DRIVER_H__
#define __ADC_DRIVER_H__

/*** Includes ***/
#include <stdint.h>

/*** Preprocessor Definitions ***/

#define ADC1_BASE_ADDR      (0x40012000UL)  /**< ADC1 base address */
#define ADC_COMMON_BASE     (0x40012300UL)  /**< ADC common (CCR) base address */
#define ADC_CHANNEL_MIN     (0U)   /**< Minimum valid channel number */
#define ADC_CHANNEL_MAX     (18U)  /**< Maximum valid channel number */
#define ADC_INJECTED_RANK_MIN   (1U)
#define ADC_INJECTED_RANK_MAX   (4U)
#define ADC_SAMPLE_TIME_DEFAULT (0x5U) /**< 84 cycles sampling time */

typedef enum
{
    ADC_OK              = 0,  /**< Operation successful              */
    ADC_ERROR_INSTANCE  = 1,  /**< Invalid ADC instance pointer      */
    ADC_ERROR_CHANNEL   = 2,  /**< Invalid channel number            */
    ADC_ERROR_RANK      = 3,  /**< Invalid injected rank             */
} ADC_Status_t;

typedef struct
{
    volatile uint32_t SR;    /**< Status register,                    offset: 0x00 */
    volatile uint32_t CR1;   /**< Control register 1,                 offset: 0x04 */
    volatile uint32_t CR2;   /**< Control register 2,                 offset: 0x08 */
    volatile uint32_t SMPR1; /**< Sample time register 1,             offset: 0x0C */
    volatile uint32_t SMPR2; /**< Sample time register 2,             offset: 0x10 */
    volatile uint32_t JOFR1; /**< Injected channel data offset reg 1, offset: 0x14 */
    volatile uint32_t JOFR2; /**< Injected channel data offset reg 2, offset: 0x18 */
    volatile uint32_t JOFR3; /**< Injected channel data offset reg 3, offset: 0x1C */
    volatile uint32_t JOFR4; /**< Injected channel data offset reg 4, offset: 0x20 */
    volatile uint32_t HTR;   /**< Watchdog higher threshold register, offset: 0x24 */
    volatile uint32_t LTR;   /**< Watchdog lower threshold register,  offset: 0x28 */
    volatile uint32_t SQR1;  /**< Regular sequence register 1,        offset: 0x2C */
    volatile uint32_t SQR2;  /**< Regular sequence register 2,        offset: 0x30 */
    volatile uint32_t SQR3;  /**< Regular sequence register 3,        offset: 0x34 */
    volatile uint32_t JSQR;  /**< Injected sequence register,         offset: 0x38 */
    volatile uint32_t JDR1;  /**< Injected data register 1,           offset: 0x3C */
    volatile uint32_t JDR2;  /**< Injected data register 2,           offset: 0x40 */
    volatile uint32_t JDR3;  /**< Injected data register 3,           offset: 0x44 */
    volatile uint32_t JDR4;  /**< Injected data register 4,           offset: 0x48 */
    volatile uint32_t DR;    /**< Regular data register,              offset: 0x4C */
} ADC_RegDef_t;

typedef struct
{
    volatile uint32_t CSR; /**< Common status register,  offset: 0x00 */
    volatile uint32_t CCR; /**< Common control register, offset: 0x04 */
    volatile uint32_t CDR; /**< Common data register,    offset: 0x08 */
} ADC_Common_RegDef_t;

/*** Global Variables ***/
/* None exposed at this level */

/*** Function Prototypes ***/

ADC_Status_t adc_init(ADC_RegDef_t *adc);

ADC_Status_t adc_enableAdc(ADC_RegDef_t *adc);

ADC_Status_t adc_setChannel(ADC_RegDef_t *adc, uint8_t channel);

ADC_Status_t adc_setInjectedChannel(ADC_RegDef_t *adc, uint8_t channel, uint8_t rank);

ADC_Status_t adc_startSingleConversion(ADC_RegDef_t *adc);

ADC_Status_t adc_startContinuousConversion(ADC_RegDef_t *adc);

ADC_Status_t adc_startInjectedConversion(ADC_RegDef_t *adc);

uint16_t adc_readData(ADC_RegDef_t *adc);

uint16_t adc_readInjectedChannelData(ADC_RegDef_t *adc, uint8_t rank);

#endif /* __ADC_DRIVER_H__ */