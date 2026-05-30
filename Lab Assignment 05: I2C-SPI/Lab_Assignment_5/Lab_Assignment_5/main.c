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
* @file main.c
* @brief Prueba MPU6050 con LED indicador (PB8/PB9, AD0 = GND -> 0x68) 
*
* @author Melanie, Michelle y Sebastian
* @date 29/04/2026
*/


#include <stdint.h>
#include <stddef.h>

#include "GPIO_stm32f4.h"
#include "I2C_Driver.h"
#include "serial.h"
#include "gy521_driver.h"

static void delay_simple(uint32_t ms) {
    volatile uint32_t i,j;
    for (i=0;i<ms;i++) for (j=0;j<12000U;j++) { /* busy */ }
}

int main(void)
{
    int r;
    uint8_t tmp;
    uint8_t buf[14];

    gpio_initPort(GPIO_PORT_A);
    gpio_setPinMode(GPIO_PORT_A, GPIO_PIN_2, GPIO_MODE_ALT_FUNC);
    gpio_setAlternateFunction(GPIO_PORT_A, GPIO_PIN_2, GPIO_AF7);
    serial_init();
    serial_printf("DEBUG: serial initialized at 115200\n");
    gpio_setPinMode(GPIO_PORT_A, GPIO_PIN_5, GPIO_MODE_OUTPUT);
    gpio_clearPin(GPIO_PORT_A, GPIO_PIN_5);

    if (gpio_initPort(GPIO_PORT_B) != GPIO_OK) {
        serial_printf("ERROR: gpio_initPort PB failed\n");
        while(1);
    }
    gpio_setPinMode(GPIO_PORT_B, GPIO_PIN_8, GPIO_MODE_ALT_FUNC);
    gpio_setAlternateFunction(GPIO_PORT_B, GPIO_PIN_8, GPIO_AF4);
    gpio_setOpenDrain(GPIO_PORT_B, GPIO_PIN_8);
    gpio_setPullUp(GPIO_PORT_B, GPIO_PIN_8);

    gpio_setPinMode(GPIO_PORT_B, GPIO_PIN_9, GPIO_MODE_ALT_FUNC);
    gpio_setAlternateFunction(GPIO_PORT_B, GPIO_PIN_9, GPIO_AF4);
    gpio_setOpenDrain(GPIO_PORT_B, GPIO_PIN_9);
    gpio_setPullUp(GPIO_PORT_B, GPIO_PIN_9);

    serial_printf("DEBUG: I2C pins configured PB8/PB9\n");

    i2c_config_t cfg = { .instance = I2C1, .clk_speed = I2C_SPEED_SM_HZ, .addr_mode = I2C_ADDR_7BIT };
    r = i2c_init(&cfg);
    serial_printf("DEBUG: i2c_init returned %d\n", r);
    if (r != I2C_OK) {
        serial_printf("ERROR: i2c_init failed, code=%d\n", r);
        while(1);
    }

    uint8_t addrs[] = {0x68U, 0x69U};
    uint8_t found = 0;
    for (size_t i = 0; i < sizeof(addrs); ++i) {
        r = i2c_readRegDevice(I2C1, addrs[i], 0x75, &tmp, 1); /* WHO_AM_I */
        serial_printf("DEBUG: read WHO_AM_I at 0x%02X -> ret=%d val=0x%02X\n", addrs[i], r, tmp);
        if (r == I2C_OK) { found = addrs[i]; break; }
    }

    if (!found) {
        serial_printf("RESULT: MPU6050 NOT FOUND at 0x68/0x69. Check: VCC=3.3V, GND common, AD0, SDA/SCL pull-ups, wiring.\n");
        while (1) { delay_simple(500); } 
    }

    serial_printf("RESULT: MPU6050 found at 0x%02X\n", found);

    r = i2c_readRegDevice(I2C1, found, MPU6050_REG_PWR_MGMT_1, &tmp, 1);
    serial_printf("DEBUG: read PWR_MGMT_1 ret=%d val=0x%02X\n", r, tmp);
    if (r == I2C_OK && tmp != 0x00) {
        uint8_t wake = 0x00;
        r = i2c_writeRegDevice(I2C1, found, MPU6050_REG_PWR_MGMT_1, &wake, 1);
        serial_printf("DEBUG: wrote 0x00 to PWR_MGMT_1 ret=%d\n", r);
        delay_simple(50);
    }

    r = i2c_readRegDevice(I2C1, found, MPU6050_REG_ACCEL_XOUT_H, buf, 14);
    serial_printf("DEBUG: read14 ret=%d\n", r);
    if (r == I2C_OK) {
        int16_t ax = (int16_t)((buf[0]<<8)|buf[1]);
        int16_t ay = (int16_t)((buf[2]<<8)|buf[3]);
        int16_t az = (int16_t)((buf[4]<<8)|buf[5]);
        serial_printf("DATA: RAW ax=%d ay=%d az=%d\n", ax, ay, az);
    } else {
        serial_printf("ERROR: read14 failed ret=%d\n", r);
    }

    gpio_setPin(GPIO_PORT_A, GPIO_PIN_5);
    while (1) {
        r = i2c_readRegDevice(I2C1, found, MPU6050_REG_ACCEL_XOUT_H, buf, 14);
        if (r == I2C_OK) {
            int16_t ax = (int16_t)((buf[0]<<8)|buf[1]);
            int16_t ay = (int16_t)((buf[2]<<8)|buf[3]);
            int16_t az = (int16_t)((buf[4]<<8)|buf[5]);
            serial_printf("DATA: ax=%d ay=%d az=%d\n", ax, ay, az);
        } else {
            serial_printf("WARN: periodic read failed ret=%d\n", r);
        }
        gpio_clearPin(GPIO_PORT_A, GPIO_PIN_5);
        delay_simple(200);
        gpio_setPin(GPIO_PORT_A, GPIO_PIN_5);
        delay_simple(200);
    }

    return 0;
}