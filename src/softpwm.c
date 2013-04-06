#include <ctype.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "softpwm.h"
#include "colors.h"

volatile HSVValue hsv;
uint8_t compare[CHMAX];

void pwm_init(void)
{
    // Starting Color/Mode
    hsv.hue=0;
    hsv.sat=0;
    hsv.val=255;
    hsv.mode=FIXED;
    hsv.speed=100;
    hsv.steps=100;


    DDRB = PORTB_MASK;             // set port pins to output
    DDRD = PORTD_MASK;

    unsigned char i=0;
    for(; i<CHMAX ; i++)       // initialise all channels
    {
        compare[i] = 0;            // set default PWM values
    }

    TIFR  |= (1 << TOV0);           // clear interrupt flag
    TIMSK |= (1 << TOIE0);         // enable overflow interrupt
    TCCR0  = (1 << CS00);           // start timer, no prescale
}

void sethsv(uint8_t hue, uint8_t sat, uint8_t val)
{
    hsv.hue=hue;
    hsv.sat=sat;
    hsv.val=val;
    hsv.steps=100;
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

        RGBValue rgb = hsv_to_rgb(hsv);

        if (hsv.steps<=0)
        {
            hsv.steps=1;
        }
        compare[0]+=(rgb.r-compare[0])/hsv.steps;
        compare[1]+=(rgb.g-compare[1])/hsv.steps;
        compare[2]+=(rgb.b-compare[2])/hsv.steps;
        hsv.steps--;


        //Color Changing
        if (++count2 == hsv.speed)
        {
            switch (hsv.mode)
            {
            case FLASH:
                hsv.hue+=100;
                hsv.steps=200;
                break;

            case FADE:
                hsv.hue+=1;
                hsv.steps=0;
                break;

            case STROBE:
                hsv.steps=1;
                if (hsv.val) hsv.val=0;
                else hsv.val=255;
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

