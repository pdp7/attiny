#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
 
int main (void)
{
  //Set PORTB.3 to be output
int i=0;
int j=0;
    DDRB = 0b11111111;
  for(i=1; i<1024; i++) {
for(j=1;j<1000;j++) {
    //Set PORTB.3 high
    PORTB = 0b11111111;
 
    _delay_ms(i);
 
    //Set PORTB.3 low again
    PORTB = 0b00000000;
 
    _delay_ms(i);
  }
_delay_ms(1000);
  }
 
  return 1;
}
