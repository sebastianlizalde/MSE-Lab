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
* @file gy521_driver.c
* @brief GY-521 (MPU-6050) sensor driver for STM32F411RE.
*
* @author Sebastian Lizalde Herrera, Michelle Urbina Carlos,
*         Melanie Picen Hernandez
* @date 2026-05-27
*/

#ifndef GY521_DRIVER_H
#define GY521_DRIVER_H

#include <stdint.h>
#include "I2C_Driver.h"

#define MPU6050_ADDR_DEFAULT    0x68U

/* Registros relevantes */
#define MPU6050_REG_PWR_MGMT_1   0x6BU
#define MPU6050_REG_SMPLRT_DIV   0x19U
#define MPU6050_REG_CONFIG       0x1AU
#define MPU6050_REG_GYRO_CONFIG  0x1BU
#define MPU6050_REG_ACCEL_CONFIG 0x1CU
#define MPU6050_REG_ACCEL_XOUT_H 0x3BU

/* Sensibilidades por defecto */
#define MPU6050_ACCEL_SENS_2G    16384.0f
#define MPU6050_GYRO_SENS_250    131.0f

/* Tipos de datos */
typedef struct {
    int16_t ax;
    int16_t ay;
    int16_t az;
    int16_t temp;
    int16_t gx;
    int16_t gy;
    int16_t gz;
} gy521_raw_t;

typedef struct {
    float ax_g;
    float ay_g;
    float az_g;
    float temp_c;
    float gx_dps;
    float gy_dps;
    float gz_dps;
} gy521_scaled_t;

int gy521_setup_addr(I2C_RegDef_t *i2c, uint8_t addr);
int gy521_setup(I2C_RegDef_t *i2c);
int gy521_fetch_raw_addr(I2C_RegDef_t *i2c, uint8_t addr, gy521_raw_t *out);
int gy521_fetch_raw(I2C_RegDef_t *i2c, gy521_raw_t *out);
int gy521_fetch_scaled_addr(I2C_RegDef_t *i2c, uint8_t addr, gy521_scaled_t *out);
int gy521_fetch_scaled(I2C_RegDef_t *i2c, gy521_scaled_t *out);

#endif /* GY521_DRIVER_H */