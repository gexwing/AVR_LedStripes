#ifndef _SOFTPWM_H_
#define _SOFTPWM_H_

#include "colors.h"

//! Pin mappings
#define CHMAX       3     // maximum number of PWM channels

#define CH0_CLEAR (pinlevelB &= ~(1 << PB0)) //R
#define CH1_CLEAR (pinlevelD &= ~(1 << PD7)) //G
#define CH2_CLEAR (pinlevelD &= ~(1 << PD6)) //B

//! Set bits corresponding to pin usage above
#define PORTB_MASK (1 << PB0)
#define PORTD_MASK (1 << PD6)|(1 << PD7)

//! prototypes
void pwm_init(void);
void sethsv(uint8_t hue, uint8_t sat, uint8_t val);

#endif //_SOFTPWM_H_
