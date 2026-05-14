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
* @file file_name.c
* @brief Brief description of what this module does.
*
* A complete description of what this module does.
*
* @author Michelle Urbina
* @date 29 de abril del 2026
*
*/

/* Includes */
#include "sensor.h"
#include "ADC_Driver.h"
#include "GPIO_stm32f4.h"
#include "Timer.h"
#include <stdint.h>

/* Preprocessor Definitions */
#define SENSOR_PORT     GPIO_PORT_A
#define SENSOR_PIN      0
#define NUM_SAMPLES     10

/* ADC1 instance */
#define ADC1  ((ADC_RegDef_t *) ADC1_BASE_ADDR)

/* Function Definitions */

void sensor_init(void)
{
    gpio_initPort(SENSOR_PORT);
    gpio_setPinMode(SENSOR_PORT, SENSOR_PIN, GPIO_MODE_ANALOG);
    adc_init(ADC1);
    adc_enableAdc(ADC1);
    adc_setChannel(ADC1, 0);
    timer_init();
}

void sensor_startConversion(void)
{
    adc_startSingleConversion(ADC1);
}

uint16_t sensor_readValue(void)
{
    long sum = 0;

    for(int i = 0; i < NUM_SAMPLES; i++)
    {
        sensor_startConversion();
        sum += adc_readData(ADC1);
        timer_delay_ms(5);
    }

    return (uint16_t)(sum / NUM_SAMPLES);
}