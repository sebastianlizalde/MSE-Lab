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
* @file main.c
* @brief 
*
*
* @author Team 6
* @date 23/04/2026
*
*/

/*** Includes ***/
#include <stdint.h>
#include "serial.h"
#include "sensor.h"
#include "Timer.h"

/*** Preprocessor Definitions ***/

#define VREF_MV         (3300U)
#define ADC_FULL_SCALE  (4095U)
#define REPORT_PERIOD_MS (500U)

/*** Function Definitions ***/

int main(void)
{
    serial_init();
    sensor_init();
    timer_init();

    serial_printf("=== UART Lab — STM32F411RE ===\n");
    serial_printf("Reporting ADC + Voltage every %d ms\n\n", REPORT_PERIOD_MS);

    while (1)
    {
        uint16_t adc_val = sensor_readValue();
                       
        uint16_t voltage_mv = (uint16_t)((uint32_t)adc_val * VREF_MV / ADC_FULL_SCALE);

        serial_printf("ADC Value: %d | Voltage: %d mV\n", adc_val, voltage_mv);

        timer_delay_ms(REPORT_PERIOD_MS);
    }

    return 0;
}