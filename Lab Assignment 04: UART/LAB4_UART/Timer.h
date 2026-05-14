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
* @file Timer.h
* @brief Timer module implementation for delay generation
*
* @author Melanie Picen
* @date 22/04/2026
*/

#ifndef TIMER_H
#define TIMER_H

/*** Includes ***/
#include <stdint.h>
#include "TIM_Driver.h"

/*** Preprocessor Definitions ***/
#define DELAY_TIMER TIM_TIMER_2

/*** Function Prototypes ***/
void timer_init(void);
void timer_delay_ms(uint32_t ms);

#endif /* TIMER_H */