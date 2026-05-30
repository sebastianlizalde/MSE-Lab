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
 * @file    I2C_Driver.h
 * @brief   Self-contained I2C Driver for STM32F411xC/E
 *
 * Defines peripheral structs, register bit masks, base addresses,
 * and the public driver API.  The only external dependency is the
 * C standard library (<stdint.h>, <stddef.h>).
 */

#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Map a peripheral base address to a typed pointer */
#define I2C_PERIPH(base)    ((I2C_RegDef_t *)(base))
#define RCC_PERIPH()        ((RCC_RegDef_t *)(RCC_BASE_ADDR))

#define I2C1_BASE_ADDR      0x40005400UL
#define I2C2_BASE_ADDR      0x40005800UL
#define I2C3_BASE_ADDR      0x40005C00UL

#define RCC_BASE_ADDR       0x40023800UL

typedef struct
{
    volatile uint32_t CR1;    /**< 0x00 — Control register 1          */
    volatile uint32_t CR2;    /**< 0x04 — Control register 2          */
    volatile uint32_t OAR1;   /**< 0x08 — Own address register 1      */
    volatile uint32_t OAR2;   /**< 0x0C — Own address register 2      */
    volatile uint32_t DR;     /**< 0x10 — Data register               */
    volatile uint32_t SR1;    /**< 0x14 — Status register 1           */
    volatile uint32_t SR2;    /**< 0x18 — Status register 2           */
    volatile uint32_t CCR;    /**< 0x1C — Clock control register      */
    volatile uint32_t TRISE;  /**< 0x20 — TRISE register              */
    volatile uint32_t FLTR;   /**< 0x24 — FLTR register               */
} I2C_RegDef_t;

/**
 * @brief  RCC peripheral register layout — APB1ENR only  (RM0383 §6.3)
 *
 * Only the registers needed to gate the I2C clocks are defined.
 * The reserved fields preserve the correct word offsets.
 */
typedef struct
{
    volatile uint32_t CR;           /**< 0x00 */
    volatile uint32_t PLLCFGR;     /**< 0x04 */
    volatile uint32_t CFGR;        /**< 0x08 */
    volatile uint32_t CIR;         /**< 0x0C */
    volatile uint32_t AHB1RSTR;    /**< 0x10 */
    volatile uint32_t AHB2RSTR;    /**< 0x14 */
    uint32_t          _res0[2];    /**< 0x18–0x1C — reserved */
    volatile uint32_t APB1RSTR;    /**< 0x20 */
    volatile uint32_t APB2RSTR;    /**< 0x24 */
    uint32_t          _res1[2];    /**< 0x28–0x2C — reserved */
    volatile uint32_t AHB1ENR;     /**< 0x30 */
    volatile uint32_t AHB2ENR;     /**< 0x34 */
    uint32_t          _res2[2];    /**< 0x38–0x3C — reserved */
    volatile uint32_t APB1ENR;     /**< 0x40 — I2C clock enable bits  */
    volatile uint32_t APB2ENR;     /**< 0x44 */
} RCC_RegDef_t;


#define I2C1    I2C_PERIPH(I2C1_BASE_ADDR)
#define I2C2    I2C_PERIPH(I2C2_BASE_ADDR)
#define I2C3    I2C_PERIPH(I2C3_BASE_ADDR)

#define I2C_CR1_PE_POS          0U
#define I2C_CR1_SMBUS_POS       1U
#define I2C_CR1_SMBTYPE_POS     3U
#define I2C_CR1_ENARP_POS       4U
#define I2C_CR1_ENPEC_POS       5U
#define I2C_CR1_ENGC_POS        6U
#define I2C_CR1_NOSTRETCH_POS   7U
#define I2C_CR1_START_POS       8U
#define I2C_CR1_STOP_POS        9U
#define I2C_CR1_ACK_POS         10U
#define I2C_CR1_POS_POS         11U
#define I2C_CR1_PEC_POS         12U
#define I2C_CR1_ALERT_POS       13U
#define I2C_CR1_SWRST_POS       15U

#define I2C_CR1_PE              (1U << I2C_CR1_PE_POS)
#define I2C_CR1_SMBUS           (1U << I2C_CR1_SMBUS_POS)
#define I2C_CR1_SMBTYPE         (1U << I2C_CR1_SMBTYPE_POS)
#define I2C_CR1_ENARP           (1U << I2C_CR1_ENARP_POS)
#define I2C_CR1_ENPEC           (1U << I2C_CR1_ENPEC_POS)
#define I2C_CR1_ENGC            (1U << I2C_CR1_ENGC_POS)
#define I2C_CR1_NOSTRETCH       (1U << I2C_CR1_NOSTRETCH_POS)
#define I2C_CR1_START           (1U << I2C_CR1_START_POS)
#define I2C_CR1_STOP            (1U << I2C_CR1_STOP_POS)
#define I2C_CR1_ACK             (1U << I2C_CR1_ACK_POS)
#define I2C_CR1_POS             (1U << I2C_CR1_POS_POS)
#define I2C_CR1_PEC             (1U << I2C_CR1_PEC_POS)
#define I2C_CR1_ALERT           (1U << I2C_CR1_ALERT_POS)
#define I2C_CR1_SWRST           (1U << I2C_CR1_SWRST_POS)

#define I2C_CR2_FREQ_POS        0U
#define I2C_CR2_ITERREN_POS     8U
#define I2C_CR2_ITEVTEN_POS     9U
#define I2C_CR2_ITBUFEN_POS     10U
#define I2C_CR2_DMAEN_POS       11U
#define I2C_CR2_LAST_POS        12U

#define I2C_CR2_FREQ_MSK        (0x3FU << I2C_CR2_FREQ_POS)
#define I2C_CR2_ITERREN         (1U << I2C_CR2_ITERREN_POS)
#define I2C_CR2_ITEVTEN         (1U << I2C_CR2_ITEVTEN_POS)
#define I2C_CR2_ITBUFEN         (1U << I2C_CR2_ITBUFEN_POS)
#define I2C_CR2_DMAEN           (1U << I2C_CR2_DMAEN_POS)
#define I2C_CR2_LAST            (1U << I2C_CR2_LAST_POS)

#define I2C_SR1_SB_POS          0U
#define I2C_SR1_ADDR_POS        1U
#define I2C_SR1_BTF_POS         2U
#define I2C_SR1_ADD10_POS       3U
#define I2C_SR1_STOPF_POS       4U
#define I2C_SR1_RXNE_POS        6U
#define I2C_SR1_TXE_POS         7U
#define I2C_SR1_BERR_POS        8U
#define I2C_SR1_ARLO_POS        9U
#define I2C_SR1_AF_POS          10U
#define I2C_SR1_OVR_POS         11U
#define I2C_SR1_PECERR_POS      12U
#define I2C_SR1_TIMEOUT_POS     14U
#define I2C_SR1_SMBALERT_POS    15U

#define I2C_SR1_SB              (1U << I2C_SR1_SB_POS)
#define I2C_SR1_ADDR            (1U << I2C_SR1_ADDR_POS)
#define I2C_SR1_BTF             (1U << I2C_SR1_BTF_POS)
#define I2C_SR1_ADD10           (1U << I2C_SR1_ADD10_POS)
#define I2C_SR1_STOPF           (1U << I2C_SR1_STOPF_POS)
#define I2C_SR1_RXNE            (1U << I2C_SR1_RXNE_POS)
#define I2C_SR1_TXE             (1U << I2C_SR1_TXE_POS)
#define I2C_SR1_BERR            (1U << I2C_SR1_BERR_POS)
#define I2C_SR1_ARLO            (1U << I2C_SR1_ARLO_POS)
#define I2C_SR1_AF              (1U << I2C_SR1_AF_POS)
#define I2C_SR1_OVR             (1U << I2C_SR1_OVR_POS)
#define I2C_SR1_PECERR          (1U << I2C_SR1_PECERR_POS)
#define I2C_SR1_TIMEOUT         (1U << I2C_SR1_TIMEOUT_POS)
#define I2C_SR1_SMBALERT        (1U << I2C_SR1_SMBALERT_POS)

#define I2C_SR1_ERR_MSK         (I2C_SR1_BERR   | \
                                 I2C_SR1_ARLO   | \
                                 I2C_SR1_AF     | \
                                 I2C_SR1_OVR    | \
                                 I2C_SR1_TIMEOUT)

#define I2C_SR2_MSL_POS         0U
#define I2C_SR2_BUSY_POS        1U
#define I2C_SR2_TRA_POS         2U
#define I2C_SR2_GENCALL_POS     4U
#define I2C_SR2_DUALF_POS       7U

#define I2C_SR2_MSL             (1U << I2C_SR2_MSL_POS)
#define I2C_SR2_BUSY            (1U << I2C_SR2_BUSY_POS)
#define I2C_SR2_TRA             (1U << I2C_SR2_TRA_POS)
#define I2C_SR2_GENCALL         (1U << I2C_SR2_GENCALL_POS)
#define I2C_SR2_DUALF           (1U << I2C_SR2_DUALF_POS)

#define I2C_CCR_CCR_POS         0U
#define I2C_CCR_DUTY_POS        14U
#define I2C_CCR_FS_POS          15U

#define I2C_CCR_CCR_MSK         (0xFFFU << I2C_CCR_CCR_POS)
#define I2C_CCR_DUTY            (1U << I2C_CCR_DUTY_POS)
#define I2C_CCR_FS              (1U << I2C_CCR_FS_POS)

#define I2C_OAR1_ADD0_POS       0U
#define I2C_OAR1_ADD_7BIT_POS   1U
#define I2C_OAR1_ADDMODE_POS    15U

#define I2C_OAR1_ADD0           (1U    << I2C_OAR1_ADD0_POS)
#define I2C_OAR1_ADD_7BIT       (0x7FU << I2C_OAR1_ADD_7BIT_POS)
#define I2C_OAR1_ADDMODE        (1U    << I2C_OAR1_ADDMODE_POS)

#define RCC_APB1ENR_I2C1EN_POS  21U
#define RCC_APB1ENR_I2C2EN_POS  22U
#define RCC_APB1ENR_I2C3EN_POS  23U

#define RCC_APB1ENR_I2C1EN      (1U << RCC_APB1ENR_I2C1EN_POS)
#define RCC_APB1ENR_I2C2EN      (1U << RCC_APB1ENR_I2C2EN_POS)
#define RCC_APB1ENR_I2C3EN      (1U << RCC_APB1ENR_I2C3EN_POS)

/** APB1 bus frequency assumed during CCR / TRISE computation (Hz) */
#define I2C_APB1_CLK_HZ     42000000U

#define I2C_SPEED_SM_HZ     100000U   /**< Standard mode — 100 kHz */
#define I2C_SPEED_FM_HZ     400000U   /**< Fast mode    — 400 kHz  */


#define I2C_ADDR_7BIT       0U   /**< 7-bit device addressing  */
#define I2C_ADDR_10BIT      1U   /**< 10-bit device addressing */


/** Polling iteration limit before a transfer is declared timed-out */
#define I2C_TIMEOUT_MAX     1000000U

typedef enum
{
    I2C_OK               =  0,   /**< Operation succeeded               */
    I2C_ERR_INVALID_ARG  = -1,   /**< NULL pointer or illegal parameter */
    I2C_ERR_BUS_BUSY     = -2,   /**< Bus not idle at START             */
    I2C_ERR_NACK         = -3,   /**< Device sent NACK                  */
    I2C_ERR_TIMEOUT      = -4,   /**< Flag wait exceeded timeout limit  */
    I2C_ERR_BUS_ERROR    = -5,   /**< Misplaced START/STOP on the bus   */
} i2c_status_t;


/**
 * @brief  Run-time configuration for i2c_init().
 */
typedef struct
{
    I2C_RegDef_t *instance;    /**< Peripheral pointer: I2C1, I2C2, I2C3 */
    uint32_t      clk_speed;   /**< Bus speed in Hz: I2C_SPEED_SM/FM_HZ  */
    uint8_t       addr_mode;   /**< I2C_ADDR_7BIT or I2C_ADDR_10BIT      */
} i2c_config_t;

i2c_status_t i2c_init(const i2c_config_t *config);


i2c_status_t i2c_writeRegDevice(I2C_RegDef_t  *instance,
                                 uint16_t       dev_addr,
                                 uint8_t        reg_addr,
                                 const uint8_t *data,
                                 size_t         len);


i2c_status_t i2c_writeDevice(I2C_RegDef_t  *instance,
                              uint16_t       dev_addr,
                              const uint8_t *data,
                              size_t         len);


i2c_status_t i2c_readRegDevice(I2C_RegDef_t *instance,
                                uint16_t      dev_addr,
                                uint8_t       reg_addr,
                                uint8_t      *buf,
                                size_t        len);


i2c_status_t i2c_readDevice(I2C_RegDef_t *instance,
                             uint16_t      dev_addr,
                             uint8_t      *buf,
                             size_t        len);

#ifdef __cplusplus
}
#endif

#endif /* I2C_DRIVER_H */