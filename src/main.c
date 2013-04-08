#include <avr/io.h>
#include <avr/interrupt.h>
#include "irmp/irmp.h"
#include "irmp/irmpconfig.h"
#include "softpwm.h"

#ifndef F_CPU
#error F_CPU unknown
#endif

#include "colors.h"
extern LED_Status led;

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

        //Repeating Commands
        if (!(irmp_data.flags&&IRMP_FLAG_REPETITION))
        {
            switch (irmp_data.command)
            {
            case 0x0B: //flash
                led.steps=0;
                if (led.mode == FLASH)
                {
                    led.mode=FIXED;
                    led.hsv.val=255;
                    led.steps=1;
                }
                else
                {
                    led.mode = FLASH;
                    led.speed = 100;
                    led.steps=1;
                }
                break;

            case 0x0F: //strobe
                led.steps=0;
                if (led.mode == STROBE)
                {
                    led.mode=FIXED;
                    led.hsv.val=255;
                }
                else
                {
                    led.mode = STROBE;
                    led.speed = 20;
                    led.hsv.sat=255;
                    led.hsv.val=255;
                }
                break;

            case 0x13: //fade
                led.steps=0;
                if (led.mode == FADE)
                {
                    led.mode=FIXED;
                    led.hsv.val=255;
                    led.speed=10;
                    led.steps=1;
                }
                else
                {
                    led.mode = FADE;
                    led.speed = 10;
                    led.hsv.sat=255;
                    led.hsv.val=255;
                    led.steps=1;
                }
                break;

            case 0x17: //smooth
                led.steps=0;
                if (led.mode == FLASH)
                {
                    led.mode=FIXED;
                    led.hsv.sat=255;
                    led.hsv.val=255;
                }
                else
                {
                    led.mode = FLASH;
                    led.speed = 0xFF;
                }
                break;
            }
        }

        //Non-Repeating Commands
        switch (irmp_data.command)
        {
            //Lightness
        case 0x00: //lighter
            if (led.hsv.val <= 245) led.hsv.val+=10;
            break;

        case 0x01: //darker
            if (led.hsv.val > 20) led.hsv.val-=10;
            break;

            //Power On/Off Maybe use idle modes?!
        case 0x02: //off
            led.hsv.val=0;
            break;

        case 0x03: //on
            led.hsv.val=255;
            break;

            //Predefined Colors
        case 0x04: //R
            led.mode=FIXED;
            sethsv(0,255,255);
            break;

        case 0x05: //G
            led.mode=FIXED;
            sethsv(85,255,255);
            break;

        case 0x06: //B
            led.mode=FIXED;
            sethsv(170,255,255);
            break;

        case 0x07: //W
            led.mode=FIXED;
            sethsv(0,0,255);
            break;

        case 0x08:
            led.mode=FIXED;
            sethsv(42,255,255);
            break;

        case 0x09:
            led.mode=FIXED;
            sethsv(127,255,255);
            break;

        case 0x0A:
            led.mode=FIXED;
            sethsv(212,255,255);
            break;

        case 0x0C: // Speed+
            if (led.speed > 1) --led.speed;
            break;

        case 0x0D:
            led.hsv.hue+=2;
            break;

        case 0x0E:
            if (led.hsv.sat<250) led.hsv.sat+=5;
            break;

        case 0x10: // Speed-
            if (led.speed < 255) ++led.speed;
            break;

        case 0x11: //hue-
            led.hsv.hue-=2;
            break;

        case 0x12:
            if (led.hsv.sat>5) led.hsv.sat-=5;
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
