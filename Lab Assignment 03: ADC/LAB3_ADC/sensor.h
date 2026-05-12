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
* @file file_name.h
* @brief Brief description of what this module does.
*
* A complete description of what this module does.
*
* @author Michelle Urbina
* @date 29 de abril del 2026
*
*/

#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>

/*** Includes ***/

/*** Preprocessor Definitions ***/

/*** Type Prototypes ***/
void sensor_init(void);

void sensor_startConversion(void);

uint16_t sensor_readValue(void);

/*** Global Variables ***/

/*** Function Prototypes ***/


#endif /* SENSOR_H */