/**************************************************************************//**
 * @file main.c
 * @brief Example that demonstrates the use of printf and other textdisplay
 *        features on the Sharp Memory LCD on the EFM32ZG_STK3200.
 * @author Energy Micro AS
 * @version 3.20.3
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silicon Labs Software License Agreement. See 
 * "http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt"  
 * for details. Before using this software for any purpose, you must agree to the 
 * terms of that agreement.
 *
 ******************************************************************************/


#include <stdio.h>
#include "em_device.h"
#include "em_cmu.h"
#include "bsp.h"

#include "display.h"
#include "textdisplay.h"
#include "retargettextdisplay.h"
#include "MPU6050.h"
#include "declarations.h"

volatile int msTicks; /* counts 1ms timeTicks */

/* Local prototypes */
void Delay(int dlyTicks);

/**************************************************************************//**
 * @brief SysTick_Handler
 * Interrupt Service Routine for system tick counter
 *****************************************************************************/
void SysTick_Handler(void)
{
  msTicks++;       /* increment counter necessary in Delay()*/
}

/**************************************************************************//**
 * @brief Delays number of msTick Systicks (typically 1 ms)
 * @param dlyTicks Number of ticks to delay
 *****************************************************************************/
void Delay(int dlyTicks)
{
  int curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) ;
}

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
  /* Setup SysTick Timer for 10 msec interrupts  */
  if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000))
  {
    while (1) ;
  }

  /* Initialize the display module. */
  DISPLAY_Init();

  /* Retarget stdio to a text display. */
  if (RETARGET_TextDisplayInit() != TEXTDISPLAY_EMSTATUS_OK)
  {
    while (1) ;
  }

  /* Output text on Memory LCD */
  printf("Hello, EFM32 Zero Gecko world!");
  Delay(2000);

  /* Clear screen */
  printf("\f");

  Setup_MPU6050();
  MPU6050_Test_I2C();
  MPU6050_Check_Registers();
  Calibrate_Gyros();
  Calibrate_Acc();
  /* Update Memory LCD display forever */
  while (1)
  {
	Get_Gyro_Rates();
	Get_Accel_Values();

	Delay(500);
	printf("\f");
  }
}
