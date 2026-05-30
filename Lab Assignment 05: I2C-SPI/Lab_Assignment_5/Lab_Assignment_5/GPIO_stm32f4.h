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
* @file GPIO_stm32f4.h
* @brief Defines the functions and peripherals to be used in GPIO Driver.
*
*
*
*/

#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stdint.h>
#include <stddef.h>

#define __I     volatile const   /**< Read-only  */
#define __O     volatile         /**< Write-only */
#define __IO    volatile         /**< Read/Write */

#ifndef RCC_BASE
#define RCC_BASE        0x40023800UL
#endif
#define RCC_AHB1ENR     (*((__IO uint32_t *)(RCC_BASE + 0x30UL)))

/** Bit positions in RCC_AHB1ENR for each GPIO port clock enable */
#define RCC_AHB1ENR_GPIOAEN_POS  (0U)
#define RCC_AHB1ENR_GPIOBEN_POS  (1U)
#define RCC_AHB1ENR_GPIOCEN_POS  (2U)
#define RCC_AHB1ENR_GPIODEN_POS  (3U)
#define RCC_AHB1ENR_GPIOEEN_POS  (4U)
#define RCC_AHB1ENR_GPIOHEN_POS  (7U)

typedef struct
{
    __IO uint32_t MODER;    /**< 0x00 – Mode register              */
    __IO uint32_t OTYPER;   /**< 0x04 – Output type register       */
    __IO uint32_t OSPEEDR;  /**< 0x08 – Output speed register      */
    __IO uint32_t PUPDR;    /**< 0x0C – Pull-up/pull-down register */
    __I  uint32_t IDR;      /**< 0x10 – Input data register        */
    __IO uint32_t ODR;      /**< 0x14 – Output data register       */
    __O  uint32_t BSRR;     /**< 0x18 – Bit set/reset register     */
    __IO uint32_t LCKR;     /**< 0x1C – Configuration lock register*/
    __IO uint32_t AFR[2];   /**< 0x20 – Alternate function regs    */
} GPIO_RegDef_t;

/** Convenience pointers – dereference to access registers directly */
#define GPIOA   ((GPIO_RegDef_t *) 0x40020000UL)
#define GPIOB   ((GPIO_RegDef_t *) 0x40020400UL)
#define GPIOC   ((GPIO_RegDef_t *) 0x40020800UL)
#define GPIOD   ((GPIO_RegDef_t *) 0x40020C00UL)
#define GPIOE   ((GPIO_RegDef_t *) 0x40021000UL)
#define GPIOH   ((GPIO_RegDef_t *) 0x40021C00UL)

typedef enum
{
    GPIO_PORT_A = 0U,
    GPIO_PORT_B = 1U,
    GPIO_PORT_C = 2U,
    GPIO_PORT_D = 3U,
    GPIO_PORT_E = 4U,
    GPIO_PORT_H = 7U,
    GPIO_PORT_COUNT    /**< Sentinel – do not use as a port value */
} gpio_port_t;


typedef enum
{
    GPIO_PIN_0  = 0U,
    GPIO_PIN_1  = 1U,
    GPIO_PIN_2  = 2U,
    GPIO_PIN_3  = 3U,
    GPIO_PIN_4  = 4U,
    GPIO_PIN_5  = 5U,
    GPIO_PIN_6  = 6U,
    GPIO_PIN_7  = 7U,
    GPIO_PIN_8  = 8U,
    GPIO_PIN_9  = 9U,
    GPIO_PIN_10 = 10U,
    GPIO_PIN_11 = 11U,
    GPIO_PIN_12 = 12U,
    GPIO_PIN_13 = 13U,
    GPIO_PIN_14 = 14U,
    GPIO_PIN_15 = 15U,
    GPIO_PIN_COUNT   /**< Sentinel – do not use as a pin value */
} gpio_pin_t;


typedef enum
{
    GPIO_MODE_INPUT     = 0x0U,  /**< Digital input          */
    GPIO_MODE_OUTPUT    = 0x1U,  /**< General-purpose output */
    GPIO_MODE_ALT_FUNC  = 0x2U,  /**< Alternate function     */
    GPIO_MODE_ANALOG    = 0x3U,   /**< Analog                 */
    GPIO_TOTAL_MODES    = 0x4U   /**< Sentinel – do not use as a mode value */
} gpio_mode_t;

typedef enum {
    GPIO_AF0 = 0U,
    GPIO_AF1 = 1U,
    GPIO_AF2 = 2U,
    GPIO_AF3 = 3U,
    GPIO_AF4  = 4U,
    GPIO_AF5 = 5U,
    GPIO_AF6 = 6U,
    GPIO_AF7 = 7U,
    GPIO_AF8 = 8U,
    GPIO_AF9 = 9U,
    GPIO_AF10 = 10U,
    GPIO_AF11 = 11U,
    GPIO_AF12 = 12U,
    GPIO_AF13 = 13U,
    GPIO_AF14 = 14U,
    GPIO_AF15 = 15U,
    GPIO_AF_COUNT = 16U
} gpio_altfunction_t;


typedef enum
{
    GPIO_OK             = 0U,   /**< Operation succeeded            */
    GPIO_ERR_INVALID_PORT = 1U, /**< Port argument out of range     */
    GPIO_ERR_INVALID_PIN = 2U,  /**< Pin argument out of range      */
    GPIO_ERR_INVALID_MODE = 3U, /**< Mode argument unrecognised     */
    GPIO_ERR_NULL_POINTER = 4U, /**< A required pointer was NULL    */
    GPIO_ERR_NOT_INITIALISED = 5U /**< Driver used before gpio_init() */
} gpio_status_t;
gpio_status_t gpio_init(void);
gpio_status_t gpio_initPort(gpio_port_t port);
gpio_status_t gpio_setPinMode(gpio_port_t port, gpio_pin_t pin, gpio_mode_t mode);
gpio_status_t gpio_setPin(gpio_port_t port, gpio_pin_t pin);
gpio_status_t gpio_clearPin(gpio_port_t port, gpio_pin_t pin);
gpio_status_t gpio_togglePin(gpio_port_t port, gpio_pin_t pin);
gpio_status_t gpio_readPin(gpio_port_t port, gpio_pin_t pin, uint8_t *state);
gpio_status_t gpio_setAlternateFunction(gpio_port_t port, gpio_pin_t pin, gpio_altfunction_t af_val);
gpio_status_t gpio_setOpenDrain(gpio_port_t port, gpio_pin_t pin);
gpio_status_t gpio_setPullUp(gpio_port_t port, gpio_pin_t pin);

#endif /* GPIO_DRIVER_H */