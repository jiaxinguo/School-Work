/*
 * XBEE.c
 *
 *  Created on: May 18, 2014
 *      Author: Jiaxin
 */

/**************************************************************************//**
 * @brief XBee Defines
 *****************************************************************************/
#include "XBEE.h"
#include "em_device.h"
#include "UART.h"

volatile uint32_t msTicks; /* counts 1ms timeTicks */

static void Delay(uint32_t dlyTicks)
{
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) ;
}

void XbeeSetup()
{
	uartSetup();
	char response[2];
	do
	{
		uartPutChar('X');
		Delay(1100);
		uartPutChar('+');
		uartPutChar('+');
		uartPutChar('+');
		Delay(1100);
		uartGetData(response, 2);
		//printf("YAY!\n");
	}
	while(response[0] != 'O' && response[1] != 'K');
	sendATCommand(setDA, destinationAddress);
	sendATCommand(setDH, 0);
	sendATCommand(setMY, myAddress);
	sendATCommand(setAP, 1);
	sendATCommand(endCommand, -1);
}

void sendATCommand(int command, int number)
{
	char cNumber[2];
	char toSend[6];
	char response[2];
	sprintf(cNumber, "%X", number);
	switch(command)
	{
		case setDA:
			do
			{
				strcpy(toSend, "ATDL");
				strcat(toSend, cNumber);
				uartPutData(toSend, 6);
				uartPutChar(CR);
				Delay(20);
				uartGetData(response, 2);
			}
			while(response[0] != 'O' || response[1] != 'K');
			break;
		case setDH:
			do
			{
				strcpy(toSend, "ATDH");
				strcat(toSend, cNumber);
				uartPutData(toSend, 6);
				uartPutChar(CR);
				Delay(20);
				uartGetData(response, 2);
			}
			while(response[0] != 'O' || response[1] != 'K');
			break;
		case setMY:
			do
			{
				strcpy(toSend, "ATMY");
				strcat(toSend, cNumber);
				uartPutData(toSend, 6);
				uartPutChar(CR);
				Delay(20);
				uartGetData(response, 2);
			}
			while(response[0] != 'O' || response[1] != 'K');
			break;
		case setAP:
			do
			{
				strcpy(toSend, "ATAP");
				strcat(toSend, cNumber);
				uartPutData(toSend, 6);
				uartPutChar(CR);
				Delay(20);
				uartGetData(response, 2);
			}
			while(response[0] != 'O' || response[1] != 'K');
			break;
		case endCommand:
			do
			{
				strcpy(toSend, "ATCN");
				uartPutData(toSend, 4);
				uartPutChar(CR);
				Delay(20);
				uartGetData(response, 2);
			}
			while(response[0] != 'O' || response[1] != 'K');
			break;
	}
}

int receiveRxFrame(char* msg, int msgMaxLength, int* msgSize)
{
	char MSG[2];
	char length;
	char copyLength;
	char source;
	char APInumber;
	char destination;
	char RSSI;
	char opt;
	char checkSum;
	char receivedCheckSum;
	int i;
	*msgSize = 0;
	checkSum = 0;
	length = uartGetChar();
	length = uartGetChar();
	copyLength = length;
	APInumber = uartGetChar();
	checkSum += APInumber;
	copyLength--;
	source = uartGetChar();
	copyLength--;
	source = uartGetChar();
	copyLength--;
	checkSum += source;
	RSSI = uartGetChar();
	copyLength--;
	checkSum += RSSI;
	opt = uartGetChar();
	copyLength--;
	checkSum += opt;
	for(i = 0; i < msgMaxLength; i++)
	{
		if(copyLength == 0)
		{
			break;
		}
		*msg = uartGetChar();
		checkSum += *msg;
		*msgSize = *msgSize + 1;
		msg++;
		copyLength--;
	}
	receivedCheckSum = uartGetChar();
	if((0xFF - checkSum == receivedCheckSum) && copyLength == 0)
	{
		return 1;
	}
	else
	{
		//printf("%d", copyLength);
		return 0;
	}
}
