#include <ctype.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "softpwm.h"
#include "colors.h"

volatile LED_Status led;
uint8_t compare[CHMAX];

void pwm_init(void)
{
    // Starting Color/Mode
    led.hsv.hue=0;
    led.hsv.sat=0;
    led.hsv.val=255;
    led.mode=FIXED;
    led.speed=100;
    led.steps=100;


    DDRB = PORTB_MASK;         // set port pins to output
    DDRD = PORTD_MASK;

    unsigned char i=0;
    for(; i<CHMAX ; i++)       // initialise all channels
    {
        compare[i] = 0;         // set default PWM values
    }

    TIFR  |= (1 << TOV0);       // clear interrupt flag
    TIMSK |= (1 << TOIE0);      // enable overflow interrupt
    TCCR0  = (1 << CS00);       // start timer, no prescale
}

void sethsv(uint8_t hue, uint8_t sat, uint8_t val)
{
    led.hsv.hue=hue;
    led.hsv.sat=sat;
    led.hsv.val=val;
    led.steps=100;
}

/*! \brief Interrupt Service Routine
 */

ISR(TIMER0_OVF_vect)
{
    static unsigned char pinlevelB=PORTB_MASK;
    static unsigned char pinlevelD=PORTD_MASK;
    static uint8_t count=0xFF;
    static uint8_t count2=0xFF;

    // update outputs
    PORTB = pinlevelB;
    PORTD = pinlevelD;

    // prepare values for next cycle
    if(++count == 0) // increment modulo 256 counter and update
    {
        // set all port pins high
        pinlevelB |= PORTB_MASK;
        pinlevelD |= PORTD_MASK;

        Color_RGB rgb = hsv_to_rgb(led.hsv);

        if (led.steps<=0)
        {
            led.steps=1;
        }
        compare[0]+=(rgb.r-compare[0])/led.steps;
        compare[1]+=(rgb.g-compare[1])/led.steps;
        compare[2]+=(rgb.b-compare[2])/led.steps;
        led.steps--;


        //Color Changing
        if (++count2 == led.speed)
        {
            switch (led.mode)
            {
            case FLASH:
                led.hsv.hue+=100;
                led.steps=200;
                break;

            case FADE:
                led.hsv.hue+=1;
                led.steps=0;
                break;

            case STROBE:
                led.steps=1;
                if (led.hsv.val) led.hsv.val=0;
                else led.hsv.val=255;
                break;
            default:
                break;
            }
            count2=0;
        }
    }


    // clear port pin on compare match (executed on next interrupt)
    if(compare[0] == count) CH0_CLEAR;
    if(compare[1] == count) CH1_CLEAR;
    if(compare[2] == count) CH2_CLEAR;
}

