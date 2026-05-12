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
* @file Timer.c
* @brief Timer module implementation for delay generation
*
* @author Melanie Picen
* @date 22/04/2026
*/

/*** Includes ***/
#include "Timer.h"

/*** Function Definitions ***/
void timer_init(void) 
{
    // Initializes the clocks for all timers (using the general function from your driver)
    // tim_init();
    
    // Prepares the specific timer we chose so it is ready from scratch
    tim_initTimer(DELAY_TIMER);
}

void timer_delay_ms(uint32_t ms) 
{
    tim_setTimerMs(DELAY_TIMER, ms);

    tim_enableTimer(DELAY_TIMER);

    tim_waitTimer(DELAY_TIMER);

    tim_disableTimer(DELAY_TIMER);
}