#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "irmp/irmp.h"
#include "irmp/irmpconfig.h"
#include "softpwm.h"

#ifndef F_CPU
#error F_CPU unknown
#endif

#include "colors.h"
extern HSVValue hsv;

void
timer_init (void)
{
    OCR1A   =  (F_CPU / F_INTERRUPTS) - 1;      // compare value: 1/10000 of CPU frequency
    TCCR1B  = (1<<WGM12)|(1<<CS10);             // switch CTC Mode on, set prescaler to 1
    TIMSK  |= (1<<OCIE1A);                      // OCIE1A: Interrupt by timer compare
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * timer 1 compare handler, called every 1/10000 sec
*---------------------------------------------------------------------------------------------------------------------------------------------------
*/

ISR(TIMER1_COMPA_vect)
{
    (void) irmp_ISR();
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * MAIN: main routine
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */

int main (void)
{
    IRMP_DATA irmp_data;

    // Setup everything
    irmp_init();
    timer_init();
    pwm_init();

    // enable interrupts
    sei();

    // Main Loop
    for(;;)
    {
        if (!irmp_get_data(&irmp_data)) continue;

        if (!(irmp_data.flags&&IRMP_FLAG_REPETITION))
        {
            switch (irmp_data.command)
            {
            case 0x0B: //flash
                hsv.steps=0;
                if (hsv.mode == FLASH)
                {
                    hsv.mode=FIXED;
                    hsv.val=255;
                    hsv.steps=1;
                }
                else
                {
                    hsv.mode = FLASH;
                    hsv.speed = 100;
                    hsv.steps=1;
                }
                break;

            case 0x0F: //strobe
                hsv.steps=0;
                if (hsv.mode == STROBE)
                {
                    hsv.mode=FIXED;
                    hsv.val=255;
                }
                else
                {
                    hsv.mode = STROBE;
                    hsv.speed = 20;
                    hsv.sat=255;
                    hsv.val=255;
                }
                break;

            case 0x13: //fade
                hsv.steps=0;
                if (hsv.mode == FADE)
                {
                    hsv.mode=FIXED;
                    hsv.val=255;
                    hsv.speed=10;
                    hsv.steps=1;
                }
                else
                {
                    hsv.mode = FADE;
                    hsv.speed = 10;
                    hsv.sat=255;
                    hsv.val=255;
                    hsv.steps=1;
                }
                break;

            case 0x17: //smooth
                hsv.steps=0;
                if (hsv.mode == FLASH)
                {
                    hsv.mode=FIXED;
                    hsv.sat=255;
                    hsv.val=255;
                }
                else
                {
                    hsv.mode = FLASH;
                    hsv.speed = 0xFF;
                }
                break;
            }
        }
        switch (irmp_data.command)
        {
            //Lightness
        case 0x00: //lighter
            if (hsv.val <= 245) hsv.val+=10;
            break;

        case 0x01: //darker
            if (hsv.val > 20) hsv.val-=10;
            break;

            //Power On/Off Maybe use idle modes?!
        case 0x02: //off
            hsv.val=0;
            break;

        case 0x03: //on
            hsv.val=255;
            break;

            //Predefined Colors
        case 0x04: //R
            hsv.mode=FIXED;
            sethsv(0,255,255);
            break;

        case 0x05: //G
            hsv.mode=FIXED;
            sethsv(85,255,255);
            break;

        case 0x06: //B
            hsv.mode=FIXED;
            sethsv(170,255,255);
            break;

        case 0x07: //W
            hsv.mode=FIXED;
            sethsv(0,0,255);
            break;

        case 0x08:
            hsv.mode=FIXED;
            sethsv(42,255,255);
            break;

        case 0x09:
            hsv.mode=FIXED;
            sethsv(127,255,255);
            break;

        case 0x0A:
            hsv.mode=FIXED;
            sethsv(212,255,255);
            break;

        case 0x0C: // Speed+
            if (hsv.speed > 1) --hsv.speed;
            break;

        case 0x0D:
            hsv.hue+=2;
            break;

        case 0x0E:
            if (hsv.sat<250) hsv.sat+=5;
            break;

        case 0x10: // Speed-
            if (hsv.speed < 255) ++hsv.speed;
            break;

        case 0x11: //hue-
            hsv.hue-=2;
            break;

        case 0x12:
            if (hsv.sat>5) hsv.sat-=5;
            break;

        case 0x14:
            break;

        case 0x15:
            break;

        case 0x16:
            break;

        }
    }
}
