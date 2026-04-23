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
* @file push_button.h
* @brief Push button driver with debounce logic.
*
* This module provides functions to initialize a push button and read its 
* state using a Finite State Machine (FSM) to filter mechanical noise.
*
* @author Melanie Picen Hernandez
* @date 15/04/2026
*
*/

#ifndef PUSH_BUTTON_H
#define PUSH_BUTTON_H

/*** Includes ***/
#include <stdint.h>

/**
 * @brief Initializes the button by configuring the corresponding port and pin.
 */
void button_init(void);

/**
 * @brief Reads the button state and applies debounce logic.
 * @return 1U if a valid press-and-release event is detected, 0U otherwise.
 */
uint8_t button_get_state(void);

#endif /* PUSH_BUTTON_H */