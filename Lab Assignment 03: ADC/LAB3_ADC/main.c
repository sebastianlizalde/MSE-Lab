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
* @brief Application entry point. Initializes GPIO, TIM2 for delay generation
*        and TIM4 for PWM output on PB6. Blinks LED on PA6 every 500ms using
*        hardware timer delays while simultaneously generating a 1kHz PWM
*        signal at 50% duty cycle.
*
*
* @author Team 6
* @date 23/04/2026
*
*/

/*** Includes ***/
#include "sensor.h"
#include "led.h"

int main(void)
{
    // Inicializar módulos
    led_init();       // configura PWM en PB6
    sensor_init();    // configura ADC en PA0

    uint16_t sensor_value = 0U;
    uint8_t  brightness   = 0U;

    while (1)
    {
        // 
        sensor_value = sensor_readValue();

        // 3. Mapear a duty cycle (0-100%)
        brightness = (uint8_t)((sensor_value * 100UL) / 4095UL);

        // 4. Aplicar brillo al LED
        led_setBrightness(brightness);
    }

    return 0;
}