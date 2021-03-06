/*****************************************************************************
 * @file
 * @brief TIMER Pulse Width Modulation Demo Application
 * @author Energy Micro AS
 * @version 1.08
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2013 Energy Micro AS, http://www.energymicro.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * 4. The source and compiled code may only be used on Energy Micro "EFM32"
 *    microcontrollers and "EFR4" radios.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Energy Micro AS has no
 * obligation to support this Software. Energy Micro AS is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Energy Micro AS will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 *****************************************************************************/
#include "em_device.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_prs.h"
#include "em_system.h"
#include "em_chip.h"
#include "em_timer.h"
#include <stdio.h>
#include "bsp.h"
#include "display.h"
#include "textdisplay.h"
#include "retargettextdisplay.h"
#include "XBEE.h"
#include "UART.h"
#include "declarations.h"

/* Define PWM frequency value */
#define PWM_FREQ 400

volatile bool pb0Pressed, pb1Pressed;

/**************************************************************************//**
 * @brief Quadcopter Defines
 *****************************************************************************/
#define DELTASPEED 50
#define MAXSPEED 28000
/**************************************************************************//**
 * @brief GPIO Interrupt handler (PC8)
 *****************************************************************************/
void GPIO_EVEN_IRQHandler(void)
{
  /* Acknowledge interrupt */
  GPIO_IntClear(1 << 8);
  pb0Pressed = true;
}

/**************************************************************************//**
 * @brief GPIO Interrupt handler (PC9)
 *****************************************************************************/
void GPIO_ODD_IRQHandler(void)
{
  /* Acknowledge interrupt */
  GPIO_IntClear(1 << 9);
  pb1Pressed = true;
}

/**************************************************************************//**
 * Delay using SysTick
 *****************************************************************************/
volatile uint32_t msTicks; /* counts 1ms timeTicks */

static void Delay(uint32_t dlyTicks)
{
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) ;
}

/**************************************************************************//**
 * @brief SysTick_Handler
 * Interrupt Service Routine for system tick counter
 *****************************************************************************/
void SysTick_Handler(void)
{
  msTicks++;       /* increment counter necessary in Delay()*/
}

/**************************************************************************//**
 * @brief TIMER0_IRQHandler
 * Interrupt Service Routine TIMER0 Interrupt Line
 *****************************************************************************/
void TIMER0_IRQHandler(void)
{
  uint32_t compareValue;
  /* Clear flag for TIMER0 overflow interrupt */
  TIMER_IntClear(TIMER0, TIMER_IF_OF);

  //compareValue = TIMER_CaptureGet(TIMER0, 0);

  /* increment duty-cycle or reset if reached TOP value */
  //if( compareValue == TIMER_TopGet(TIMER0))
    TIMER_CompareBufSet(TIMER0, 0, 0);
  //else
    //TIMER_CompareBufSet(TIMER0, 0, ++compareValue);
}

/**************************************************************************//**
 * @brief Setup GPIO interrupts for push buttons
 *****************************************************************************/
static void onBoardButtonSetup(void)
{
  /* Enable GPIO in CMU */
  //CMU_ClockEnable(cmuClock_GPIO, true);

  /* Configure PC8 and PC9 as input */
  GPIO_PinModeSet(gpioPortC, 8, gpioModeInput, 0);
  GPIO_PinModeSet(gpioPortC, 9, gpioModeInput, 0);

  /* Set falling edge interrupt for both ports */
  GPIO_IntConfig(gpioPortC, 8, false, true, true);
  GPIO_IntConfig(gpioPortC, 9, false, true, true);

  /* Enable interrupt in core for even and odd gpio interrupts */
  NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);

  NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
  NVIC_EnableIRQ(GPIO_ODD_IRQn);

  return;
}

/**************************************************************************//**
 * @brief  Main function
 * Main is called from __iar_program_start, see assembly startup file
 *****************************************************************************/
int main(void)
{
	int currentSpeed = 14000;
    /* Initialize chip */
    CHIP_Init();
    if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1) ;
    BSP_Init(BSP_INIT_DEFAULT);
    BSP_LedsInit();
    /* Enable clock for GPIO module */
    CMU_ClockEnable(cmuClock_GPIO, true);

    /* Enable clock for TIMER0 module */
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_TIMER0, true);
    CMU_ClockEnable(cmuClock_TIMER1, true);
    CMU_ClockEnable(cmuClock_USART1, true);

    /* Set CC0 location 3 pin (PD1) as output */
    GPIO_PinModeSet(gpioPortA, 0, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortA, 1, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortC, 13, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortC, 14, gpioModePushPull, 0);
    onBoardButtonSetup();

	/* Initialize the display module. */
	DISPLAY_Init();

	/* Retarget stdio to a text display. */
	if (RETARGET_TextDisplayInit() != TEXTDISPLAY_EMSTATUS_OK)
	{
	 while (1) ;
	}
	printf("Hello, EFM32 Zero Gecko world!\n");

	/* Initialize UART and XBEE */
	BSP_LedsSet(0x0001);
	XbeeSetup();
	BSP_LedsSet(0x0002);
	while(1)
	{
		if(pb0Pressed)
		{
		  BSP_LedsSet(0x000f);
		  //printf("Starting QuadCopter!\n");
		  pb0Pressed = false;
		  /* Select CC channel parameters */
		  TIMER_InitCC_TypeDef timerCCInit =
		  {
			.eventCtrl  = timerEventEveryEdge,
			.edge       = timerEdgeBoth,
			.prsSel     = timerPRSSELCh0,
			.cufoa      = timerOutputActionNone,
			.cofoa      = timerOutputActionNone,
			.cmoa       = timerOutputActionNone,
			.mode       = timerCCModePWM,
			.filter     = false,
			.prsInput   = false,
			.coist      = false,
			.outInvert  = false,
		  };

		  /* Configure CC channel 0 */
		  TIMER_InitCC(TIMER0, 0, &timerCCInit);
		  TIMER_InitCC(TIMER1, 0, &timerCCInit);
		  timerCCInit.prsSel = timerPRSSELCh1;
		  TIMER_InitCC(TIMER0, 1, &timerCCInit);
		  TIMER_InitCC(TIMER1, 1, &timerCCInit);

		  /* Route CC0 to location 3 (PD1) and enable pin */
		  TIMER0->ROUTE |= (TIMER_ROUTE_CC0PEN | TIMER_ROUTE_LOCATION_LOC0);
		  TIMER0->ROUTE |= (TIMER_ROUTE_CC1PEN | TIMER_ROUTE_LOCATION_LOC0);
		  TIMER1->ROUTE |= (TIMER_ROUTE_CC0PEN | TIMER_ROUTE_LOCATION_LOC0);
		  TIMER1->ROUTE |= (TIMER_ROUTE_CC1PEN | TIMER_ROUTE_LOCATION_LOC0);

		  /* Set Top Value */
		  TIMER_TopSet(TIMER0, CMU_ClockFreqGet(cmuClock_HFPER)/PWM_FREQ);
		  TIMER_TopSet(TIMER1, CMU_ClockFreqGet(cmuClock_HFPER)/PWM_FREQ);

		  /* Set compare value starting at 0 - it will be incremented in the interrupt handler */
		  TIMER_CompareBufSet(TIMER0, 0, 14000);
		  TIMER_CompareBufSet(TIMER0, 1, 14000);
		  TIMER_CompareBufSet(TIMER1, 0, 14000);
		  TIMER_CompareBufSet(TIMER1, 1, 14000);

		  /* Select timer parameters */
		  TIMER_Init_TypeDef timerInit =
		  {
			.enable     = true,
			.debugRun   = true,
			.prescale   = timerPrescale1,
			.clkSel     = timerClkSelHFPerClk,
			.fallAction = timerInputActionNone,
			.riseAction = timerInputActionNone,
			.mode       = timerModeUp,
			.dmaClrAct  = false,
			.quadModeX4 = false,
			.oneShot    = false,
			.sync       = false,
		  };

		  /* Enable overflow interrupt */
		  //TIMER_IntEnable(TIMER0, TIMER_IF_OF);

		  /* Enable TIMER0 interrupt vector in NVIC */
		 // NVIC_EnableIRQ(TIMER0_IRQn);

		  /* Configure timer */
		  TIMER_Init(TIMER0, &timerInit);
		  TIMER_Init(TIMER1, &timerInit);
		  Delay(250);

		  while(1)
		  {
			if(!pb1Pressed)
			{
				char checkDelimiter = uartGetChar();
				if(checkDelimiter == 0x7E)
				{
					//BSP_LedsSet(0x0002);
					char msg = 0;
					int msgSize = 0;
					int status = receiveRxFrame(&msg, 1, &msgSize);
					if(status)
					{
						if(msg == 'u')
						{
							//BSP_LedsSet(0x000f);
							if(currentSpeed + 700 <= 28000)
							{
								BSP_LedsSet(0x000f);
								currentSpeed = currentSpeed + 700;

								TIMER_CompareBufSet(TIMER0, 0, currentSpeed); //red 1 closer to mcu wire
								TIMER_CompareBufSet(TIMER0, 1, currentSpeed);   //white 1 closer to mcu wire
								TIMER_CompareBufSet(TIMER1, 0, currentSpeed);	//red 2
								TIMER_CompareBufSet(TIMER1, 1, currentSpeed);	//white 2
							}
						}
						else if(msg == 'd')
						{
							//BSP_LedsSet(0x0001);
							if(currentSpeed - 700 >= 14000)
							{
								BSP_LedsSet(0x0001);
								currentSpeed = currentSpeed - 700;

								TIMER_CompareBufSet(TIMER0, 0, currentSpeed);
								TIMER_CompareBufSet(TIMER0, 1, currentSpeed);
								TIMER_CompareBufSet(TIMER1, 0, currentSpeed);
								TIMER_CompareBufSet(TIMER1, 1, currentSpeed);
							}
						}
						else if(msg == 'l')
						{
							//BSP_LedsSet(0x0002);
						}
						else
						{
							//BSP_LedsSet(0x0000);
						}
					}
				}
				Delay(3);
			}
			else
			{
				//printf("Stopped QuadCopter!\n");
				pb1Pressed = false;
				BSP_LedsSet(0x000f);
				TIMER_CompareBufSet(TIMER0, 0, 14000);
				TIMER_CompareBufSet(TIMER0, 1, 14000);
				TIMER_CompareBufSet(TIMER1, 0, 14000);
				TIMER_CompareBufSet(TIMER1, 1, 14000);
				while(1);
			}
		  }
		}
	}

}

