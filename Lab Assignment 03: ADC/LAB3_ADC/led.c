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
* @file led.c
* @brief Implementation of LED control functions.
*
* 
*
* @author 
* @date 
*
*/

/*** Includes ***/
#include "led.h"
#include "pwm.h"

/*** Preprocessor Definitions ***/
#define LED_PWM_FREQUENCY  1000U   // 1kHz

/*** Function Definitions ***/

void led_init(void)
{
    // El PWM configura PB6 internamente
    pwm_init();
}

void led_on(void)
{
    pwm_setSignal(LED_PWM_FREQUENCY, 100U);
    pwm_start();
}

void led_off(void)
{
    pwm_stop();
}

void led_toggle(void)
{
    // No aplica en modo PWM
}

void led_setBrightness(uint8_t duty_cycle)
{
        pwm_setSignal(LED_PWM_FREQUENCY, duty_cycle);
        pwm_start();
}