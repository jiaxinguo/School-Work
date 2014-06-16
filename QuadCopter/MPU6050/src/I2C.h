/*
 * I2C.h
 *
 *  Created on: May 28, 2014
 *      Author: Jiaxin
 */

#ifndef I2C_H_
#define I2C_H_

#include "em_i2c.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "bsp.h"

void I2C0_IRQHandler(void);
void I2C_Setup(void);
int I2C_RegisterSet(I2C_TypeDef *i2c,
                         uint8_t addr,
                         uint8_t reg,
                         uint8_t val);
int I2C_RegisterGet(I2C_TypeDef *i2c,
                         uint8_t addr,
                         uint8_t reg,
                         uint8_t *val,
                         uint32_t len);

#endif /* I2C_H_ */
