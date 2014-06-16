/*
 * XBEE.h
 *
 *  Created on: May 18, 2014
 *      Author: Jiaxin
 */

#ifndef XBEE_H_
#define XBEE_H_

#define setDA 0
#define setDH 1
#define setMY 2
#define setAP 3
#define setIU 4
#define endCommand 5
#define setRE 6
#define CR   0x0D

#define myAddress 23

#define destinationAddress 79

void XbeeSetup();
void checkResponse();
int receiveRxFrame(char* msg, int msgMaxLength, int* msgSize);
void sendATCommand(int command, int number);

#endif /* XBEE_H_ */
