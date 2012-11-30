/*
 * This file is part of a project posted at Dangerous Prototypes (http://dangerousprototypes.com)
 * Project: Christmas card ornament '09
 *
 * To the extent possible under law, Dangerous Prototypes (Ian Lesnet) has
 * waived all copyright and related or neighboring rights to this code. This
 * work is published from United States.
 *
 * For details see: http://creativecommons.org/publicdomain/zero/1.0/.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <avr/io.h>
#include <avr/sleep.h>

#define LED_CONTROL_PIN _BV(3)
#define NOP __asm__ __volatile__ ("nop")

void delay_cnt(unsigned char c, int d);

int main(void){
	unsigned char i, j, n, d;

	//configure chip
	//select clock source
	DDRB=0;	//ctrl pin output
	PORTB=0xff;				//all pins GND

	for(i=0; i<10; i++){ 		//flash the lights 10 times

		//we don't just blink them, we fade them in and out
		for(j=0; j<0xff; j++){	//fade in gently
			PORTB=0xFF;		//LEDs on
			delay_cnt(j,d);		//delay (grows)
			PORTB=0;		//LEDs off
			delay_cnt(0xff-j,d);	//delay (shrinks)
		}
		
		for(j=0; j<0xff; j++){	//fade out gently
			PORTB=0x00;		//LEDs off
			delay_cnt(j,d);		//delay (grows)
			PORTB=0xFF;		//LEDs on
			delay_cnt(0xff-j,d);	//delay (shrinks)
		}

	}

	PORTB=0x00;				//all pins GND
        MCUCR = _BV(SM1) |  _BV(SE);//configure sleep mode 
        sleep_cpu();    

}

//simple time wasting delay function
void delay_cnt(unsigned char c, int d){ 
	unsigned char i;
	unsigned char j;

	for(i=0; i<c; i++) { 
	for(j=0; j<(c/2); j++) { 
NOP; //waste time
}}}
