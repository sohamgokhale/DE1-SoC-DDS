# DE1-SoC-DDS
---

Library for DE1-SoC board to generate musical notes using Direct Digital Synthesis (DDS) method. This library was made for University of Leeds ELEC5620M Module coursework.

## Features
* Sine Wave Tone generation.
* Play musical notes directly by calling the DDS_note function.

## File Descriptions
| File  | Purpose |
| ---   | --- |
|`DDS.c`,`.h`| DDS driver to generate tones using WM8731 Audio Codec on DE-1 SoC |
|`WM8731.c`,`.h`| Custom driver for  WM8731 Audio codec |
|`PrivateTimer.c`,`.h`| Driver for ARM Private Timer to run in free-running mode. Provides millis() function that returns number of milliseconds passed from last system reset|

## Requirements
* The WM8731.c driver utilises the HPS_I2C library provided by UoL during the Module.
* PrivateTimer.c utilises the HPS_IRQ library provided by UoL during the Module.

## Credits
* [Prachiti Kulkarni](https://github.com/prachitikulkarni) for Private Timer files.