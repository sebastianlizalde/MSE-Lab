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
* Implements gy521_setup, gy521_setup_addr, and gy521_fetch_raw / gy521_fetch_scaled.
* Reads 14 consecutive bytes starting from ACCEL_XOUT_H (0x3B):
*   [0-1]  accel X   [2-3]  accel Y   [4-5]  accel Z
*   [6-7]  temperature
*   [8-9]  gyro X   [10-11] gyro Y   [12-13] gyro Z
*
* @author Sebastian Lizalde Herrera, Michelle Urbina Carlos,
*         Melanie Picen Hernandez
* @date 2026-05-27
*/


#include "gy521_driver.h"
#include "I2C_Driver.h"
#include <stdint.h>

/* Helpers I2C: devuelven 0 OK, -1 error */
static int imu_write_reg(I2C_RegDef_t *i2c, uint8_t addr, uint8_t reg, uint8_t val)
{
    if (i2c == NULL) return -1;
    if (i2c_writeRegDevice(i2c, addr, reg, &val, 1) != I2C_OK) return -1;
    return 0;
}

static int imu_read_regs(I2C_RegDef_t *i2c, uint8_t addr, uint8_t reg, uint8_t *buf, size_t len)
{
    if (i2c == NULL || buf == NULL) return -1;
    if (i2c_readRegDevice(i2c, addr, reg, buf, len) != I2C_OK) return -1;
    return 0;
}

/* Small busy-wait delay (no inline asm) */
static void imu_delay(uint32_t iterations)
{
    volatile uint32_t i;
    for (i = 0; i < iterations; ++i) {
        /* empty loop for delay */
    }
}

/* Inicialización con dirección explícita */
int gy521_setup_addr(I2C_RegDef_t *i2c, uint8_t addr)
{
    if (i2c == NULL) return -1;

    /* Wake up: clear sleep bit */
    if (imu_write_reg(i2c, addr, MPU6050_REG_PWR_MGMT_1, 0x00) != 0) return -1;

    /* Small busy-wait delay to let oscillator stabilise (replace with platform delay if available) */
    imu_delay(200000U);

    /* Readback to confirm device responded */
    uint8_t readback = 0xFF;
    if (imu_read_regs(i2c, addr, MPU6050_REG_PWR_MGMT_1, &readback, 1) != 0) return -1;

    /* Configure sample rate, DLPF, full-scale ranges */
    if (imu_write_reg(i2c, addr, MPU6050_REG_SMPLRT_DIV, 0x07) != 0) return -1; /* 125 Hz */
    if (imu_write_reg(i2c, addr, MPU6050_REG_CONFIG, 0x03) != 0) return -1;     /* DLPF = 3 */
    if (imu_write_reg(i2c, addr, MPU6050_REG_GYRO_CONFIG, 0x00) != 0) return -1;/* ±250 dps */
    if (imu_write_reg(i2c, addr, MPU6050_REG_ACCEL_CONFIG, 0x00) != 0) return -1;/* ±2 g */

    return 0;
}

int gy521_setup(I2C_RegDef_t *i2c)
{
    return gy521_setup_addr(i2c, MPU6050_ADDR_DEFAULT);
}

/* Lectura raw de 14 bytes desde dirección dada */
int gy521_fetch_raw_addr(I2C_RegDef_t *i2c, uint8_t addr, gy521_raw_t *out)
{
    if (i2c == NULL || out == NULL) return -1;

    uint8_t buf[14];
    if (imu_read_regs(i2c, addr, MPU6050_REG_ACCEL_XOUT_H, buf, 14) != 0) return -1;

    out->ax   = (int16_t)((buf[0] << 8) | buf[1]);
    out->ay   = (int16_t)((buf[2] << 8) | buf[3]);
    out->az   = (int16_t)((buf[4] << 8) | buf[5]);
    out->temp = (int16_t)((buf[6] << 8) | buf[7]);
    out->gx   = (int16_t)((buf[8] << 8) | buf[9]);
    out->gy   = (int16_t)((buf[10] << 8) | buf[11]);
    out->gz   = (int16_t)((buf[12] << 8) | buf[13]);

    return 0;
}

int gy521_fetch_raw(I2C_RegDef_t *i2c, gy521_raw_t *out)
{
    return gy521_fetch_raw_addr(i2c, MPU6050_ADDR_DEFAULT, out);
}

/* Lectura escalada (g, °/s, °C) */
int gy521_fetch_scaled_addr(I2C_RegDef_t *i2c, uint8_t addr, gy521_scaled_t *out)
{
    if (i2c == NULL || out == NULL) return -1;

    gy521_raw_t r;
    if (gy521_fetch_raw_addr(i2c, addr, &r) != 0) return -1;

    out->ax_g   = (float)r.ax / MPU6050_ACCEL_SENS_2G;
    out->ay_g   = (float)r.ay / MPU6050_ACCEL_SENS_2G;
    out->az_g   = (float)r.az / MPU6050_ACCEL_SENS_2G;

    /* Temperature: Temp_degC = (TEMP_OUT / 340) + 36.53 */
    out->temp_c = ((float)r.temp / 340.0f) + 36.53f;

    out->gx_dps = (float)r.gx / MPU6050_GYRO_SENS_250;
    out->gy_dps = (float)r.gy / MPU6050_GYRO_SENS_250;
    out->gz_dps = (float)r.gz / MPU6050_GYRO_SENS_250;

    return 0;
}

int gy521_fetch_scaled(I2C_RegDef_t *i2c, gy521_scaled_t *out)
{
    return gy521_fetch_scaled_addr(i2c, MPU6050_ADDR_DEFAULT, out);
}