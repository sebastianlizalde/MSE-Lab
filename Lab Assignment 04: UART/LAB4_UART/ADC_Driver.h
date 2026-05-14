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

#define ADC1_BASE_ADDR      (0x40012000UL)  
#define ADC_COMMON_BASE     (0x40012300UL)  
#define ADC_CHANNEL_MIN     (0U)   
#define ADC_CHANNEL_MAX     (18U)  
#define ADC_INJECTED_RANK_MIN   (1U)
#define ADC_INJECTED_RANK_MAX   (4U)
#define ADC_SAMPLE_TIME_DEFAULT (0x5U)

typedef enum
{
    ADC_OK              = 0,  
    ADC_ERROR_INSTANCE  = 1,  
    ADC_ERROR_CHANNEL   = 2,  
    ADC_ERROR_RANK      = 3,  
} ADC_Status_t;

typedef struct
{
    volatile uint32_t SR;    
    volatile uint32_t CR1;   
    volatile uint32_t CR2;   
    volatile uint32_t SMPR1; 
    volatile uint32_t SMPR2; 
    volatile uint32_t JOFR1; 
    volatile uint32_t JOFR2; 
    volatile uint32_t JOFR3; 
    volatile uint32_t JOFR4; 
    volatile uint32_t HTR;  
    volatile uint32_t LTR;   
    volatile uint32_t SQR1; 
    volatile uint32_t SQR2;  
    volatile uint32_t SQR3;  
    volatile uint32_t JSQR;  
    volatile uint32_t JDR1;  
    volatile uint32_t JDR2;  
    volatile uint32_t JDR3;  
    volatile uint32_t JDR4;  
    volatile uint32_t DR;   
} ADC_RegDef_t;

typedef struct
{
    volatile uint32_t CSR; 
    volatile uint32_t CCR; 
    volatile uint32_t CDR; 
} ADC_Common_RegDef_t;

/*** Global Variables ***/


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