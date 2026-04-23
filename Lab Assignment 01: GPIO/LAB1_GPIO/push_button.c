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
* @file push.c
* @brief Implementation of the push button debounce and state logic.
*
* This file implements a State Machine to confirm button presses, ensuring 
* that events are only triggered once the button has been released.
*
* @author Melanie Picen Hernandez
* @date 15/04/2026
*
*/

/*** Includes ***/
#include "push_button.h"
#include "GPIO_stm32f4.h"

/*** Preprocessor Definitions ***/
#define BUTTON_PORT  GPIO_PORT_C
#define BUTTON_PIN   13U

/*** Type Prototypes ***/
typedef enum
{
    BUTTON_IDLE,
    BUTTON_PRESSING,
    BUTTON_CONFIRMED,
    BUTTON_RELEASED
} ButtonState_t;

/*** Local Variables ***/

/*** External Variables ***/

/*** Function Prototypes ***/

/*** Function Definitions ***/
void button_init(void)
{
    /* Initialize the clock for the port and set pin as input */
    gpio_initPort(BUTTON_PORT);
    gpio_setPinMode(BUTTON_PORT, BUTTON_PIN, GPIO_MODE_INPUT);
}

uint8_t button_get_state(void)
{
    static ButtonState_t state = BUTTON_IDLE;
    static uint32_t count = 0U;
    uint8_t pinVal = 1U;

    /* Read the current digital value of the pin */
    gpio_readPin(BUTTON_PORT, BUTTON_PIN, &pinVal);

    switch(state)
    {
        case BUTTON_IDLE:
            if(pinVal == 0U)
            {
                count = 0U;
                state = BUTTON_PRESSING;
            }
            break;

        case BUTTON_PRESSING:
            if(pinVal == 0U)
            {
                count++;
                if(count >= 50U)
                    state = BUTTON_CONFIRMED;
            }
            else
            {
                state = BUTTON_IDLE;
            }
            break;

        case BUTTON_CONFIRMED:
            if(pinVal == 1U)
                state = BUTTON_RELEASED;
            break;

        case BUTTON_RELEASED:
            state = BUTTON_IDLE;
            return 1U;
    }
    return 0U;
}