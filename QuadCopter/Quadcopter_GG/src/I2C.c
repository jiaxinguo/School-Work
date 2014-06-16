/*
 * I2C.c
 *
 *  Created on: May 28, 2014
 *      Author: Jiaxin
 */

#include "I2C.h"

/*******************************************************************************
 **************************   GLOBAL VARIABLES   *******************************
 ******************************************************************************/
I2C_TransferReturn_TypeDef I2C_Status;

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/**************************************************************************//**
 * @brief I2C Interrupt Handler.
 *        The interrupt table is in assembly startup file startup_efm32.s
 *****************************************************************************/
void I2C1_IRQHandler(void)
{
  /* Just run the I2C_Transfer function that checks interrupts flags and returns */
  /* the appropriate status */
//  I2C_IntClear(I2C1, I2C_IFC_TXC);
  I2C_Status = I2C_Transfer(I2C1);
}

void I2C_Setup(void)
{

  /* Initialize I2C driver for the tempsensor on the DK, using standard rate. */
  /* Devices on DK itself supports fast mode, */
  /* but in case some slower devices are added on */
  /* prototype board, we use standard mode. */
  I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;
  i2cInit.freq = 400000;
  i2cInit.clhr = i2cClockHLRFast;

  //CMU_ClockEnable(cmuClock_GPIO, true);
  //CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_I2C1, true);

  /* Use location 3: SDA - Pin D14, SCL - Pin D15 */
  /* Output value must be set to 1 to not drive lines low... We set */
  /* SCL first, to ensure it is high before changing SDA. */
  GPIO_PinModeSet(gpioPortE, 0, gpioModeWiredAnd, 1);
  GPIO_PinModeSet(gpioPortE, 1, gpioModeWiredAnd, 1);

  /* Enable pins at location 3 (which is used on the DVK) */
  I2C1->ROUTE = I2C_ROUTE_SDAPEN |
                I2C_ROUTE_SCLPEN |
                (2 << _I2C_ROUTE_LOCATION_SHIFT);

  I2C_Init(I2C1, &i2cInit);

  /* Clear and enable interrupt from I2C module */
//  I2C_IntClear(I2C1, I2C_IEN_TXC);
//  I2C_IntEnable(I2C1, I2C_IEN_TXC);
  //NVIC_ClearPendingIRQ(I2C1_IRQn);
  //NVIC_EnableIRQ(I2C1_IRQn);
}

/***************************************************************************//**
 * @brief
 *   Write to sensor register.
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address for temperature sensor, in 8 bit format, where LSB is reserved
 *   for R/W bit.
 *
 * @param[in] reg
 *   Register to write (temperature register cannot be written).
 *
 * @param[in] val
 *   Value used when writing to register.
 *
 * @return
 *   Returns 0 if register written, <0 if unable to write to register.
 ******************************************************************************/
int I2C_RegisterSet(I2C_TypeDef *i2c,
                         uint8_t addr,
                         uint8_t reg,
                         uint8_t val)
{
  I2C_TransferSeq_TypeDef seq;
  uint8_t data[1];
  uint8_t data2[1];

  seq.addr = addr;
  seq.flags = I2C_FLAG_WRITE_WRITE;
  /* Select register to be written */
  data[0] = reg;
  seq.buf[0].data = data;
  /* Only 1 byte reg */
  data2[0] = val;
  seq.buf[0].len = 1;
  seq.buf[1].data = data2;
  seq.buf[1].len = 1;

  /* Do a polled transfer */
  I2C_Status = I2C_TransferInit(i2c, &seq);

  /* Sending data */
  while (I2C_Transfer(I2C1) != i2cTransferDone){;}
//
//  while (I2C_Status != i2cTransferDone)
//  {
//    /* Enter EM1 while waiting for I2C interrupt */
//	EMU_EnterEM1();
//    /* Could do a timeout function here. */
//  }

  return(0);
}


/***************************************************************************//**
 * @brief
 *   Read sensor register content.
 *
 * @details
 *   If reading the temperature register, when a measurement is completed inside
 *   the sensor device, the new measurement may not be stored. For this reason,
 *   the temperature should not be polled with a higher frequency than the
 *   measurement conversion time for a given resolution configuration. Please
 *   refer to sensor device datasheet.
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address for temperature sensor, in 8 bit format, where LSB is reserved
 *   for R/W bit.
 *
 * @param[in] reg
 *   Register to read.
 *
 * @param[out] val
 *   Reference to place register read.
 *
 * @return
 *   Returns 0 if register read, <0 if unable to read register.
 ******************************************************************************/
int I2C_RegisterGet(I2C_TypeDef *i2c,
                         uint8_t addr,
                         uint8_t reg,
                         uint8_t *val,
                         uint32_t len)
{
  I2C_TransferSeq_TypeDef seq;
  uint8_t regid[1];
  uint8_t data[1];

  seq.addr = addr;
  seq.flags = I2C_FLAG_WRITE_READ;
  /* Select register to be read */
  regid[0] = ((uint8_t)reg);
  seq.buf[0].data = regid;
  seq.buf[0].len = 1;
  /* Only 1 byte reg*/
  seq.buf[1].data = data;
  seq.buf[1].len = 1;


  /* Do a polled transfer */
  I2C_Status = I2C_TransferInit(i2c, &seq);
  /* Sending data */
  while (I2C_Transfer(I2C1) != i2cTransferDone){;}

  //if (I2C_Status != i2cTransferDone)
  //{
    //return((int)I2C_Status);
  //}

  *val = data[0];

  return(0);
}


