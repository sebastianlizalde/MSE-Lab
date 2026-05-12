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
* @file led.h
* @brief LED control driver abstraction.

*
*/

#ifndef LED_H
#define LED_H

/*** Includes ***/
#include <stdint.h>

/*** Function Prototypes ***/

void led_init(void);
void led_on(void);
void led_off(void);
void led_toggle(void);
void led_setBrightness(uint8_t duty_cycle);

#endif /* __LED_H__ */