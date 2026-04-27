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
#include "GPIO_stm32f4.h"
#include "TIM_Driver.h"
#include "led.h"
#include "Timer.h"
#include "pwm.h"

int main(void)
{
    /*** Initialization ***/

    gpio_init();

    timer_init();

    led_init();

    pwm_init();

    pwm_setSignal(1000, 50);

    pwm_start();

    /*** Main Loop ***/
    while (1)
    {
        led_on();
        timer_delay_ms(500);

        led_off();
        timer_delay_ms(500);

    }

    return 0;
}