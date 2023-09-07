/*
 * WM8731 Audio Codec Driver for DE-1 SoC
 * ----------------------------
 * By: Soham Gokhale
 * For: University of Leeds - ELEC5620M - Mini Project Group 18
 * Date: 25th Apr 2023
 *
 * Description
 * ------------
 * This file is an improved driver for WM8731 Audio CODEC that is
 * on board the DE-1 SoC. This files is based on the driver provided
 * in the UoL ELEC5620M-Resources repo and aims to make it more
 * suitable to our application.
 *
 * Files Required:
 * --------------
 * 		HPS_I2C.c, HPS_I2C.h: Driver files for the HPS_I2C
 *
 */

// Includes //
#include "HPS_I2C/HPS_I2C.h"
#include "WM8731.h"

// I2C Register Addresses
#define WM8731_I2C_LEFTINCNTRL		0x00u
#define WM8731_I2C_RIGHTINCNTRL 	0x02u
#define WM8731_I2C_LEFTOUTCNTRL  	0x04u
#define WM8731_I2C_RIGHTOUTCNTRL 	0x06u
#define WM8731_I2C_ANLGPATHCNTRL 	0x08u
#define WM8731_I2C_DGTLPATHCNTRL 	0x0Au
#define WM8731_I2C_POWERCNTRL    	0x0Cu
#define WM8731_I2C_DATAFMTCNTRL  	0x0Eu
#define WM8731_I2C_SMPLINGCNTRL  	0x10u
#define WM8731_I2C_ACTIVECNTRL   	0x12u

// I2C Masks for register addr and data
#define WM8731_I2C_ADDR_MASK		0xFE00
#define WM8731_I2C_DATA_MASK		0x01FF

// I2C Initialisation parameters
#define WM8731_I2C_DEVICE_ADDR		0x1Au
#define WM8731_I2C_CTRL_ID			0x0u

// Driver Initialised
static bool wm8731_init = false;

// Create variable to hold arm audio register structure
volatile ARM_AUDIO_t arm_audio;

// Function to Initialise Audio Controller
signed int WM8731_init() {
	signed int status;

	//Ensure I2C Controller "I2C1" is initialised
	if (!HPS_I2C_isInitialised(0)) {
		status = HPS_I2C_initialise(0);
		if (status != HPS_I2C_SUCCESS)
			return status;
	}

	//Initialise the WM8731 codec over I2C.

	//Power Down Output, Line-In, ADC and MIC
	status = WM8731_setReg(WM8731_I2C_POWERCNTRL, 	0x17 );
	if (status != HPS_I2C_SUCCESS) return status;
	//Mute Both Input Channels.
	status = WM8731_setReg(WM8731_I2C_LEFTINCNTRL, 	0x180);
	if (status != HPS_I2C_SUCCESS) return status;
	//+6dB Volume. Unmute. Simultaneous load both channels
	status = WM8731_setReg(WM8731_I2C_LEFTOUTCNTRL, 0x179);
	if (status != HPS_I2C_SUCCESS) return status;
	//Use Line In. Disable Bypass. Use DAC
	status = WM8731_setReg(WM8731_I2C_ANLGPATHCNTRL, 0x12);
	if (status != HPS_I2C_SUCCESS) return status;
	//Enable High-Pass filter. 48kHz sample rate.
	status = WM8731_setReg(WM8731_I2C_DGTLPATHCNTRL, 0x06);
	if (status != HPS_I2C_SUCCESS) return status;
	//I2S Mode, 24bit, Master Mode (do not change this!)
	status = WM8731_setReg(WM8731_I2C_DATAFMTCNTRL,  0x4E);
	if (status != HPS_I2C_SUCCESS) return status;
	//Normal Mode, 48kHz sample rate
	status = WM8731_setReg(WM8731_I2C_SMPLINGCNTRL,	 0x00);
	if (status != HPS_I2C_SUCCESS) return status;
	//Enable Codec
	status = WM8731_setReg(WM8731_I2C_ACTIVECNTRL,	 0x01);
	if (status != HPS_I2C_SUCCESS) return status;
	//Power-up output.
	status = WM8731_setReg(WM8731_I2C_POWERCNTRL,	 0x07);
	if (status != HPS_I2C_SUCCESS) return status;

	//Mark as initialised so later functions know we are ready
	wm8731_init = true;
	//Clear the audio FIFOs
	return WM8731_clearBuffer(true, true);
}

// Function to check if driver initialised
bool WM8731_isInit() {
	return wm8731_init;
}

// Function to clear FIFO Buffer of ADC and DAC
signed int WM8731_clearBuffer( bool adc, bool dac) {
	// Read current status of control register
	unsigned int ctrl = arm_audio.reg.control;

	// Check if WM8731 has been initialised
	if (!WM8731_isInit())
		return WM8731_ERRORNOINIT; //not initialised

	// Set respective bits
	if (adc)
		ctrl |= (1 << 2);
	if (dac)
		ctrl |= (1 << 3);
	arm_audio.reg.control = ctrl;

	// Clear respective bits
	if (adc)
		ctrl &= ~(1 << 2);
	if (dac)
		ctrl &= ~(1 << 3);
	arm_audio.reg.control = ctrl;

	return WM8731_SUCCESS;
}

// Static function to write data to Audio Codec Registers over I2C
static signed int WM8731_setReg(unsigned short reg, unsigned short data) {

	unsigned short i2c_data = 0;
	// Ensure I2C Controller "I2C1" is initialised
	if (!HPS_I2C_isInitialised(0)) return HPS_I2C_ERRORNOINIT;

	// 16 bit value. Upper 7-bits are address. Lower 9-bits are data.
	// Address shifted by 8-bits, upper 7-bits masked
	// ORed with (lower 9-bits of data masked)
	i2c_data = ((reg << 8) & WM8731_I2C_ADDR_MASK)
			| (data & WM8731_I2C_DATA_MASK);

	// Write generated value over I2C and return status
	return HPS_I2C_write16b(WM8731_I2C_CTRL_ID,
			WM8731_I2C_DEVICE_ADDR,
			i2c_data);
}

// Function to write sample to left channel
signed int WM8731_writeL(unsigned int sample){
	if (!WM8731_isInit()) return WM8731_ERRORNOINIT;
	if (arm_audio.fifo_space.WSLC == 0) return WM8731_FIFOFULL;
	arm_audio.reg.left_data = sample;
	return WM8731_SUCCESS;
}

// Function to write sample to right channel
signed int WM8731_writeR(unsigned int sample){
	if (!WM8731_isInit()) return WM8731_ERRORNOINIT;
	if (arm_audio.fifo_space.WSRC == 0) return WM8731_FIFOFULL;
	arm_audio.reg.right_data = sample;
	return WM8731_SUCCESS;
}
