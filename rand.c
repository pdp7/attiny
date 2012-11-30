/*
* DigitalCandle -- Version 1
* -----------
* Use random numbers to emulate a flickering candle on a single LED without hardware PWM
*
* David M. Cole <cole@dmcole.net>
* License: 2009 Creative Commons Attribution-Noncommercial-Share Alike 3.0 U.S.
* Target: ATTiny13
* Compiler: AVR-GCC
*/
 
#include <avr/io.h>														// Defines pins, po
 
//	Declare variables
//	==============================================
	uint8_t x, y, z, i, j;
//	==============================================
 
	int main(void)
		{
			DDRB = 0b11111111;										// PortB all outputs
 			for(;;)												// loop forever
 				{
		 			x = 1;
					y = rand(1,255);
					z = rand(63,255);
					for (i = z; i < y; i++)								// the fading in loop
						{
							PORTB = 1 << x; 						// light on
							for (j=0; j < y; j++)
								{
									if (j > i)
										{
											PORTB = 0 << x;	 		// light off
										}
								}
							}								// end fade in
					y = rand(1,255);
					z = rand(63,255);
					for (i = y; i > z; i--)								// the fading out loop
						{
							PORTB = 1 << x;							// light on
							for (j=0; j < y; j++)
								{
									if (j > i)
										{
											PORTB = 0 << x;			// light off
										}
								}
						}									// end fade out
				}
		}

