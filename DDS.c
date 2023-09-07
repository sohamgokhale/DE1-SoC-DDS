/*
 * DDS driver for WM8731 Audio Codec on DE-1 SoC
 * ----------------------------
 * By: Soham Gokhale
 * For: University of Leeds - ELEC5620M
 * Date: 26th Apr 2023
 *
 * Description
 * ------------
 * This file has functions to create waveforms using
 * Direct Digital Synthesis (DDS)
 *
 */

#include "PrivateTimer.h"
#include "WM8731.h"
#include "DDS.h"

#include <math.h>

//Define some useful constants
#define F_SAMPLE 48000.0        //Sampling rate of WM8731 Codec (Do not change)
#define PI2      6.28318530718  //2 x Pi
#define AMPL	 26843545.0

// Enumeration of musical notes to index frequency from the array
typedef enum {C, Db, D, Eb, E, F, Gb, G, Ab, A, Bb, B} notes_t;
notes_t note;

// Constant array containing frequency of each note
const double note_freq[] = {261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.00, 415.30, 440, 466.16, 493.88};

// Structure for Numerically Controlled Oscillator
typedef struct{
	double phase;  // Phase accumulator
	double inc;  // Phase increment
}NCO_t;

// Function to generate sine wave of desired duration in milliseconds
void DDS_sineWave(double freq, unsigned long duration){
	unsigned long start_t = millis();
	signed int sample;
	NCO_t nco;
	nco.inc = freq * PI2 / F_SAMPLE;
	while(millis()-start_t < duration){
		nco.phase += nco.inc;
		while (nco.phase >= PI2) nco.phase -= PI2;
		sample = (signed int) (AMPL * sin(nco.phase));
		WM8731_writeL(sample);
		WM8731_writeR(sample);
	}
}

// Function to play a musical note
void DDS_note(notes_t note, unsigned short octave, unsigned long duration){
	// Calculate the multiplier for the required octave
	signed short power = octave - 4;
	double freq;
	freq = note_freq[note] * pow(2,power); // Calculate actual frequency
	DDS_sineWave(freq,duration);
}


// Example Sound Effect 1
void DDS_sfx1(){
	DDS_note(C,3,125);
	DDS_note(G,3,200);
}

// Example Sound Effect 2
void DDS_sfx2(){
	DDS_note(C,4,125);
	DDS_note(G,4,125);
	DDS_note(C,5,200);
}
