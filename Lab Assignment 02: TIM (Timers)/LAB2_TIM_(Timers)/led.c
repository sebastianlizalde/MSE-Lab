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
* Handles the initialization of the GPIO port and pin, and provides functions
* to set, clear, and toggle the LED state.
*
* @author Michelle Urbina
* @date 04/2026
*
*/

/*** Includes ***/
#include "led.h"
#include "GPIO_stm32f4.h"

/*** Preprocessor Definitions ***/
#define LED_PORT  GPIO_PORT_A
#define LED_PIN   6U  

/*** Function Definitions ***/

void led_init(void)
{
    /* Enable the clock for Port A */
    gpio_initPort(LED_PORT);

    /* Set pin 1 as output (sending signal) */
    gpio_setPinMode(LED_PORT, LED_PIN, GPIO_MODE_OUTPUT);
}

void led_on(void)
{
    /* Send 1 -> Turn LED ON */
    gpio_setPin(LED_PORT, LED_PIN);
}

void led_off(void)
{
    /* Send 0 -> Turn LED OFF */
    gpio_clearPin(LED_PORT, LED_PIN);
}

void led_toggle(void)
{
    /* Toggle current state */
    gpio_togglePin(LED_PORT, LED_PIN);
}