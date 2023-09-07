/*
 * PrivateTimer.h
 *
 *  Created on: 25-Apr-2023
 *      Author: Prachiti Kulkarni
 *  Header file for PrivateTimer.c
 */


#ifndef PRIVATETIMER_H_
#define PRIVATETIMER_H_

#include "HPS_IRQ/HPS_IRQ.h"
#include "HPS_Watchdog/HPS_Watchdog.h"


// Structure for private timer registers
typedef struct{
	unsigned int load;
	unsigned int current;
	unsigned int control;
	unsigned int interrupt;
}PRIVATE_TIMER_t;

// The structure variable is creaed at the base address of private timer
extern volatile PRIVATE_TIMER_t private_timer __attribute__((at(0xFFFEC600)));

void private_timer_init(void);

void private_timer_reset(void);

void private_timer_interrupt_handler(HPSIRQSource interruptID, bool isInit, void* initParams);

unsigned long int millis(void);

#endif /* PRIVATETIMER_H_ */
