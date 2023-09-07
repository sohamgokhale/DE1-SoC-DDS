#ifndef DDS_H_
#define DDS_H_

//Include required header files
#include <stdbool.h> //Boolean variable type "bool" and "true"/"false" constants.

extern enum notes note;
void DDS_sineWave(unsigned short, unsigned long);
void DDS_note(unsigned char note, unsigned short octave, unsigned long duration);
void DDS_sfx1();
void DDS_sfx2();
void DDS_sfx3();
void DDS_sfx4();

#endif /*DDS_H_*/
