/*
 * WM8731 Audio Codec Driver for DE-1 SoC
 * ----------------------------
 * By: Soham Gokhale
 * For: University of Leeds - ELEC5620M - Mini Project Group 18
 * Date: 25th Apr 2023
 *
 * Description
 * ------------
 * Header file for WM8731 Audio CODEC driver.
 *
 */

#ifndef WM8731_H_
#define WM8731_H_

//Include required header files
#include <stdbool.h> //Boolean variable type "bool" and "true"/"false" constants.

//Error Codes
#define WM8731_SUCCESS      0
#define WM8731_ERRORNOINIT -1
#define WM8731_FIFOFULL	   -2

// ARM WM8736 Register
typedef union{
	// bitfield definitions
	struct{
		unsigned int  :32;
		unsigned char RARC;
		unsigned char RALC;
		unsigned char WSRC;
		unsigned char WSLC;
	}fifo_space;
	// register definitions
	struct{
		unsigned int control;
		unsigned int fifo_space;
		unsigned int left_data;
		unsigned int right_data;
	}reg;
}ARM_AUDIO_t;
// Variable defined at base address
extern volatile ARM_AUDIO_t arm_audio __attribute__((at(0xFF203040)));

//Initialise Audio Codec
// - returns 0 if successful
signed int WM8731_init ( void );

//Check if driver initialised
// - Returns true if driver previously initialised
// - WM8731_init() must be called if false.
bool WM8731_isInit ( void );

// Clears FIFO Buffers
// returns 0 if successful
signed int WM8731_clearBuffer( bool, bool);

// Static function to write data to CODEC registers via I2C
static signed int WM8731_setReg (unsigned short reg, unsigned short data);

// Functions to write data samples to each channel
signed int WM8731_writeL(unsigned int );
signed int WM8731_writeR(unsigned int );

#endif /*WM8731_H_*/
