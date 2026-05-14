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
* @file ADC_Driver.c
* @brief ADC driver implementation for STM32F411RE.
*
* This module implements all ADC driver functions defined in adc_driver.h.
* It configures the ADC1 peripheral on the STM32F411RE.
*
*
* @author Sebastian Lizalde Herrera
* @date 2026-04-29
*/

/*** Includes ***/
#include "adc_driver.h"

/*** Preprocessor Definitions ***/

#define RCC_APB2ENR         (*((volatile uint32_t *)0x40023844UL))
#define RCC_APB2ENR_ADC1EN  (1U << 8)
#define ADC_SR_EOC          (1U << 1)  
#define ADC_SR_JEOC         (1U << 2)  
#define ADC_SR_STRT         (1U << 4) 
#define ADC_CR1_RES_12BIT   (0U << 24) 
#define ADC_CR1_SCAN_DIS    (0U << 8) 
#define ADC_CR2_ADON        (1U << 0)  
#define ADC_CR2_CONT        (1U << 1)  
#define ADC_CR2_ALIGN_RIGHT (0U << 11) 
#define ADC_CR2_SWSTART     (1U << 30) 
#define ADC_CR2_JSWSTART    (1U << 22) 
#define ADC_CR2_JEXTEN_DIS  (0U << 20) 
#define ADC_CR2_EXTEN_DIS   (0U << 28) 
#define ADC_CCR_ADCPRE_DIV4 (1U << 16) 
#define ADC_SQR1_L_1CONV    (0U << 20)
#define ADC_JSQR_JL_1CONV   (0U << 20)
#define ADC_JSQR_JSQ4_POS   (10U)
#define ADC_STABILIZATION_DELAY (300U)

/*** Type Prototypes ***/

/*** Local Variables ***/

/*** External Variables ***/

/*** Function Prototypes ***/

static void     adc_setSampleTime(ADC_RegDef_t *adc, uint8_t channel, uint8_t sampleTime);
static uint32_t adc_getStabilizationDelay(void);

/*** Function Definitions ***/

static void adc_setSampleTime(ADC_RegDef_t *adc, uint8_t channel, uint8_t sampleTime)
{
    if (channel <= 9U)
    {
        adc->SMPR2 &= ~(0x7U << (channel * 3U));
        adc->SMPR2 |=  ((uint32_t)sampleTime << (channel * 3U));
    }
    else
    {
        uint8_t shift = (channel - 10U) * 3U;
        adc->SMPR1 &= ~(0x7U << shift);
        adc->SMPR1 |=  ((uint32_t)sampleTime << shift);
    }
}

static uint32_t adc_getStabilizationDelay(void)
{
    volatile uint32_t count = ADC_STABILIZATION_DELAY;
    while (count > 0U)
    {
        count--;
    }
    return count;
}

ADC_Status_t adc_init(ADC_RegDef_t *adc)
{
    if (adc == ((void *)0))
    {
        return ADC_ERROR_INSTANCE;
    }

    RCC_APB2ENR |= RCC_APB2ENR_ADC1EN;

    ADC_Common_RegDef_t *adcCommon = (ADC_Common_RegDef_t *)ADC_COMMON_BASE;
    adcCommon->CCR &= ~(0x3U << 16);
    adcCommon->CCR |= ADC_CCR_ADCPRE_DIV4;

    adc->CR1 &= ~(0x3U << 24); 
    adc->CR1 |= ADC_CR1_RES_12BIT;
    adc->CR1 &= ~(1U << 8);    

    adc->CR2 &= ~(1U << 11);   
    adc->CR2 &= ~(0x3U << 28); 
    adc->CR2 &= ~(0x3U << 20); 

    adc->SQR1 &= ~(0xFU << 20);
    adc->SQR1 |= ADC_SQR1_L_1CONV;

    return ADC_OK;
}

ADC_Status_t adc_enableAdc(ADC_RegDef_t *adc)
{
    if (adc == ((void *)0))
    {
        return ADC_ERROR_INSTANCE;
    }

    adc->CR2 |= ADC_CR2_ADON;

    (void)adc_getStabilizationDelay();

    return ADC_OK;
}

ADC_Status_t adc_setChannel(ADC_RegDef_t *adc, uint8_t channel)
{
    if (adc == ((void *)0))
    {
        return ADC_ERROR_INSTANCE;
    }

    if (channel > ADC_CHANNEL_MAX)
    {
        return ADC_ERROR_CHANNEL;
    }

    adc->SQR3 &= ~(0x1FU << 0);
    adc->SQR3 |=  ((uint32_t)channel << 0);

    adc_setSampleTime(adc, channel, ADC_SAMPLE_TIME_DEFAULT);

    return ADC_OK;
}

ADC_Status_t adc_setInjectedChannel(ADC_RegDef_t *adc, uint8_t channel, uint8_t rank)
{
    if (adc == ((void *)0))
    {
        return ADC_ERROR_INSTANCE;
    }

    if (channel > ADC_CHANNEL_MAX)
    {
        return ADC_ERROR_CHANNEL;
    }

    if ((rank < ADC_INJECTED_RANK_MIN) || (rank > ADC_INJECTED_RANK_MAX))
    {
        return ADC_ERROR_RANK;
    }

    adc->JSQR &= ~(0x3U << 20);      
    adc->JSQR &= ~(0x1FU << ADC_JSQR_JSQ4_POS);
    adc->JSQR |=  ((uint32_t)channel << ADC_JSQR_JSQ4_POS);

    adc_setSampleTime(adc, channel, ADC_SAMPLE_TIME_DEFAULT);

    return ADC_OK;
}

ADC_Status_t adc_startSingleConversion(ADC_RegDef_t *adc)
{
    if (adc == ((void *)0))
    {
        return ADC_ERROR_INSTANCE;
    }

    adc->CR2 &= ~ADC_CR2_CONT;

    adc->SR &= ~(ADC_SR_EOC | ADC_SR_STRT);

    adc->CR2 |= ADC_CR2_SWSTART;

    return ADC_OK;
}

ADC_Status_t adc_startContinuousConversion(ADC_RegDef_t *adc)
{
    if (adc == ((void *)0))
    {
        return ADC_ERROR_INSTANCE;
    }

    adc->CR2 |= ADC_CR2_CONT;

    adc->SR &= ~ADC_SR_EOC;

    adc->CR2 |= ADC_CR2_SWSTART;

    return ADC_OK;
}

ADC_Status_t adc_startInjectedConversion(ADC_RegDef_t *adc)
{
    if (adc == ((void *)0))
    {
        return ADC_ERROR_INSTANCE;
    }

    adc->SR &= ~ADC_SR_JEOC;

    adc->CR2 |= ADC_CR2_JSWSTART;

    return ADC_OK;
}

uint16_t adc_readData(ADC_RegDef_t *adc)
{
    if (adc == ((void *)0))
    {
        return 0xFFFFU;
    }

    while (!(adc->SR & ADC_SR_EOC))
    {
        
    }

    return (uint16_t)(adc->DR & 0x0FFFU);
}

uint16_t adc_readInjectedChannelData(ADC_RegDef_t *adc, uint8_t rank)
{
    uint16_t result = 0xFFFFU;

    if (adc == ((void *)0))
    {
        return result;
    }

    if ((rank < ADC_INJECTED_RANK_MIN) || (rank > ADC_INJECTED_RANK_MAX))
    {
        return result;
    }

    while (!(adc->SR & ADC_SR_JEOC))
    {
        
    }

    adc->SR &= ~ADC_SR_JEOC;

    switch (rank)
    {
        case 1U:  result = (uint16_t)(adc->JDR1 & 0xFFFFU); break;
        case 2U:  result = (uint16_t)(adc->JDR2 & 0xFFFFU); break;
        case 3U:  result = (uint16_t)(adc->JDR3 & 0xFFFFU); break;
        case 4U:  result = (uint16_t)(adc->JDR4 & 0xFFFFU); break;
        default:  break;
    }

    return result;
}