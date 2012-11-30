#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN PB4

int main(void) {
int i=2;
  // Configure LED_PIN (PB4) as output
  DDRB |= (1 << LED_PIN);

for(;;) {
    PORTB |= (1 << LED_PIN);
    _delay_ms(i);
    PORTB &= ~(1 << LED_PIN);
    _delay_ms(i);
  }

  return 0;
} 
