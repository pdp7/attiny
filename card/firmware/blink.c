#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN PB4

int main(void) {
double i=500;
double j=1100000;
int c =0;
for(;;){
DDRB |= (1 << LED_PIN);
for(c=0;c<50;c++) {
    PORTB |= (1 << LED_PIN);
    _delay_us(j);
    PORTB &= ~(1 << LED_PIN);
    _delay_us(j);
}
_delay_ms(4000);
}

return 0;
} 
