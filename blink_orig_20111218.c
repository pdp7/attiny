
/*
 *  firelamp.c      PWM control of LEDs on an ATtiny85
 */

#include  <avr/io.h>
#include  <avr/pgmspace.h>
#include  <avr/interrupt.h>


#define  NUM_LEDS            4
#define  MASK_LEDS            4       /* assumes PB0 - PB3 */

#define  NUM_PWM_STATES        32
#define  MAX_PWM_STATE        (NUM_PWM_STATES-1)

#define  PORT_LEDS            PORTB
#define  DDR_LEDS            DDRB

#define  DELAY                1000UL        /* general delay in tics */




const unsigned long int        pwmvals[NUM_PWM_STATES] PROGMEM =
    {
        0x00000000L,  0x00010000L,  0x00010001L,  0x80200800L,
        0x01010101L,  0x82080820L,  0x84108410L,  0x84112410L,
        0x11111111L,  0x11249111L,  0x12491249L,  0x25225252L,
        0x25252525L,  0x25525522L,  0x25552555L,  0x25555555L,
        0x55555555L,  0x55575555L,  0x57555755L,  0x57575755L,
        0x57575757L,  0x57577775L,  0x57777577L,  0x57777777L,
        0x77777777L,  0xf7777777L,  0xf777f777L,  0xf7f7f777L,
        0xf7f7f7f7L,  0xf7fff7f7L,  0xf7fffff7L,  0xf0000000L
    };





uint8_t                        bright[NUM_LEDS];
uint8_t                        delta[NUM_LEDS];
uint32_t                    pwm[NUM_LEDS];
uint16_t                    delays[NUM_LEDS];
volatile uint16_t            tics[NUM_LEDS];



/*
 *  Local functions
 */

uint16_t                    readtics(uint8_t  cntr);
void                        writetics(uint8_t  cntr, uint16_t  delay);
void                        assignpwm(uint8_t  led);
static long unsigned int      b_random(void);
static long unsigned int      rnd(long unsigned int  val);



int  main(void)
{
    uint8_t                    n;
    unsigned long int        nval;

    TCCR0B = (1<<CS01);                        // /8 prescaler
    TIMSK = (1<<TOIE0);                        // enable interrupt on TOF

    PORT_LEDS = PORT_LEDS & ~MASK_LEDS;        // turn off all LEDs
    DDR_LEDS = DDR_LEDS | MASK_LEDS;        // make all LED drive lines outputs

    for (n=0; n<NUM_LEDS; n++)
    {
        bright[n] = 0;                        // start with all brightness values at 0
        assignpwm(n);                        // make it so
        delays[n] = 200;                    // start with an arbitray delay value
    } 

    sei();                                    // turn on interrupts


    while (1)                                // main loop
    {
        for (n=0; n<NUM_LEDS; n++)            // for each LED
        {
            if (readtics(n) == 0)            // if done with current delay for this LED...
            {
                nval = rnd(20) + 11;
                bright[n] = nval & 0xff;    // update the brightness
                assignpwm(n);

                nval = rnd(750) + 250;        // calc a random delay
                writetics(n, nval & 0xffff);
            }
        }
    }
    return  0;
}



void  assignpwm(uint8_t  led)
{
    cli();                                    // do not disturb
    pwm[led] = pgm_read_dword(&pwmvals[bright[led]]);
    sei();
}



uint16_t  readtics(uint8_t  cntr)
{
    uint16_t            t;

    t = tics[cntr];
    if (t != tics[cntr])  t = tics[cntr];
    return  t;
}



void  writetics(uint8_t  cntr, uint16_t  delay)
{
    cli();
    tics[cntr] = delay;
    sei();
}




/*
 *  The following functions try to duplicate the ANSI random() function for 8-bit MCUs such as the
 *  Atmel ATmega1284p.  Seed is fixed at compile time.
 */
static long unsigned int  b_random(void)
{
    static long unsigned int                seed = 12345678L;

    seed = 1664525L * seed + 1013904223L;
    return seed;
}



static long unsigned int  rnd(long unsigned int  val)
{
    long unsigned int                    t;

    t = b_random();                            // compute a 32-bit random number
    val = t % val + 1L;                        // now keep it within requested range
    return  val;
}






SIGNAL(TIM0_OVF_vect)
{
    uint8_t            n;
    uint8_t            mask;
    uint32_t        t;

    mask = 0;
    for (n=0; n<NUM_LEDS; n++)                // for LED 0 through NUM_LED-1...
    {
        t = pwm[n] & 1;                        // get low bit of PWM for this LED
        mask = mask | (t << n);                // move low bit of PWM into proper bit of mask
        pwm[n] = (pwm[n] >> 1);                // move PWM value one bit to the right
        if (t)  pwm[n] = pwm[n] + 0x80000000;    // rotate original low bit into high bit
        if (tics[n]) tics[n]--;                // drop this counter if not yet 0
    }

    PORT_LEDS = PORT_LEDS & ~(MASK_LEDS);    // strip off port lines dedicated to LEDs
    PORT_LEDS = PORT_LEDS | mask;            // turn on the LEDs
}


