/**************************************************************************//**
 * @file main.c
 * @brief Space Invaders Game for EFM32ZG_STK3200
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


#include "em_device.h"
#include "em_emu.h"
#include "em_cmu.h"
#include "spaceinvaders.h"


/************************************************************
 **                                                        **
 **   SYSTICK interrupt handler                            **
 **                                                        **
 ************************************************************/
void SysTick_Handler( void ) {

   SPACEINVADERS_TimerEventHandler();
}


/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
   /* Enable HFXO as the main clock */
   CMU_ClockSelectSet( cmuClock_HF, cmuSelect_HFXO );

   /* Setup SysTick Timer for 1 msec interrupts  */
   if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1) ;
   
   /* Initiate game */
   SPACEINVADERS_Init();
   
   /* Enter game loop. Will not return */
   SPACEINVADERS_GameLoop();  

   return 0;
}
