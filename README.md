AVR_LedStripes
==============

Overview
-------
This Project uses an Atmega8 microcontroller to control RGB Led Stripes. The goal of this project is to have a nice modular platform with various modules to control the Color via IR Remote, Sound, or maybe other methods in the future.

src/ - The sourcecode for the Atmege, written in C.  
docs/ - documentation for hardware parts and application notes.  
eagle/ - CadSoft Eagle Schematics and Board Layout Files.  
TLSpice/ - Simulation files to test parts of the board schematics.  

Modules
-------
IRMP - Infrared Multiprotocol decoder  
SoftPWM - Software Pulse-Width-Modulation with 3 Channels via Timer Interrupt

TODO
-------
[x] Bring code upto date for v2 Board  
[] investigate gamma correction for PWM Module  
[] Add a microphone with preamp to the board for sound sampling.  
[] Add a FFT Module for anazyling sound spectrums.  
[] Add Mode for Sounds Controled Lighting  

Links
-------
Using Lookup Tables for Gamma Correction on LEDs: http://www.maximintegrated.com/app-notes/index.mvp/id/3667  
Fixed-Point FFT for Atmega: http://elm-chan.org/works/akilcd/report_e.html  
Infrared-Multiprotocol-Decoder: http://www.mikrocontroller.net/articles/IRMP  

License
-------
TBA
