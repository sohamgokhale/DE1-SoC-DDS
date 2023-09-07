/*
 * PrivateTimer.c
 *
 *  Created on: 25-Apr-2023
 *      Author: Prachiti Kulkarni
 *
 *  This module is a free running timer and can be used
 *  for generating non-blocking delays
 */


#include "PrivateTimer.h"
#include "HPS_IRQ/HPS_IRQ.h"

volatile  PRIVATE_TIMER_t private_timer;
unsigned long int counter = 0;

// This function is called to initialize ARM A9 Private Timer
void private_timer_init(void)
{
// Reset function is called to modify load and control register
	private_timer_reset();
// Private Timer Interrupt handler is initialized
	HPS_IRQ_registerHandler(IRQ_MPCORE_PRIVATE_TIMER, private_timer_interrupt_handler);
}

// This function is used to reset the private timer
void private_timer_reset(void)
{
	private_timer.load    = 225000; //To generate 1ms delay
	private_timer.control = 0x07;   /* Flag E, I and A are set to enable timer,
									   enable interrupt and enable auto reload*/
}

/* This is a interrupt handler function for private timer.
   On the timer overflow counter is updated by 1*/
void private_timer_interrupt_handler(HPSIRQSource interruptID, bool isInit, void* initParams)
{
	// Check if interrupt is handled
	if(!isInit)
	{
			counter++;						//Increment the vounter
			private_timer.interrupt = 0x1;  //Clear 'F' flag by writing 1
	}
	HPS_ResetWatchdog();
}

// This returns the counter value which holds value of milliseconds since last system reset
unsigned long millis(void)
{
	return counter;
}
