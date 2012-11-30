// Happy Birthday Card by Drew Fustini:
//     http://us.element14.com/fustini
// Modified from musicbox1.c by NerdKits:
//     http://www.nerdkits.com/videos/musicbox1/

#define F_CPU 9600000UL	//9.6MHz

#include <avr/io.h>
#include <avr/sleep.h>

// Frequencies from http://www.phy.mtu.edu/~suits/notefreqs.html
// converted to half-periods (us) by calculating
// 1000000/2/frequency where frequency is in Hz
// note: additionally adjusted to compenstate for ATtiny13
#define D5 1050
#define E5 935
#define Fsh5 833
#define G5 786
#define A5 701
#define A4 1402
#define B5 624
#define C6 588
#define D6 524

//Note Durations
#define DUR2 110
#define DUR4 55
#define DUR8D 40
#define DUR16 13

#define nop() asm volatile("nop") 

//Delay specified number of microseconds
//warning: 1 "microsecond" = 1.23 real microseconds
void delay_us(uint16_t delay) {
	uint16_t i = 0;
	for(i=0;i<delay;i++) { 
		nop(); 
	}
}

//Plays specified frequency for a specified duration
void play_tone(uint16_t delay, uint8_t duration) {

	// delay is half-period in microseconds
	// duration is in 10ms increments

	// duration = 2*delay * cycles (all in same units)
	// cycles = 10000 * duration / delay / 2
	// cycles = 100 * duration / (delay/50)
  
	uint16_t tmp = 100 * duration;
	uint16_t delaysm = delay / 50;
	uint16_t cycles = tmp / delaysm;

	while(cycles > 0) {
		PORTB = 0xFF; //set speaker high (ideally could be more granular)
		delay_us(delay);
		PORTB = 0x00; //set speaker low
		delay_us(delay);
		cycles--;
	}

	delay_us(0xFFFF);

}



int main() {

	delay_us(0xFFFF); //brief delay at startup
  	DDRB = 0xFF; //set pins to output, ideally should be more granular

	//Play "Happy Birthday" one time
	play_tone(D5, DUR8D); 
	play_tone(D5, DUR16);
	play_tone(E5, DUR4);
	play_tone(D5, DUR4);
	play_tone(G5, DUR4);
	play_tone(Fsh5, DUR2);
	play_tone(D5, DUR8D);
	play_tone(D5, DUR16);
	play_tone(E5, DUR4);
	play_tone(D5, DUR4);
	play_tone(A5, DUR4);
	play_tone(G5, DUR2);
	play_tone(D5, DUR8D); 
	play_tone(D5, DUR16);
	play_tone(D6, DUR4);
	play_tone(B5, DUR4);
	play_tone(G5, DUR4);
	play_tone(Fsh5, DUR4);
	play_tone(E5, DUR2);
	play_tone(C6, DUR8D); 
	play_tone(C6, DUR16);
	play_tone(B5, DUR4);
	play_tone(G5, DUR4);
	play_tone(A5, DUR4);
	play_tone(G5, DUR2);

	PORTB=0x00; //all pins GND
	DDRB=0xFF; //all pin states Input
        MCUCR = _BV(SM1) |  _BV(SE); //configure sleep mode 
        sleep_cpu(); //sleep until button pressed again  

  	return 0;

}
