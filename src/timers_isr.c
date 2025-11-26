#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "dimmer_digital/dimmer_defs.h"

ISR(TIMER2_OVF_vect)
{
    seconds++;
    if(seconds == 60){
        seconds = 0;
        minutes++;
        if(minutes == 60){
            minutes = 0;
            hours++;
            if(hours == 24){
                hours = 0;
            }
        }
    }
}

ISR(TIMER1_COMPA_vect)
{
	switch(display){
		case 0:
			PORTB &= ~_BV(_enable_dez);
			PORTB &= ~_BV(_dado00) & ~_BV(_dado01) & ~_BV(_dado02) & ~_BV(_dado03);
			PORTB |= _BV(_enable_uni);
			PORTB |= unit;
			display = 1;
			break;
		case 1:
			PORTB &= ~_BV(_enable_uni);
			PORTB &= ~_BV(_dado00) & ~_BV(_dado01) & ~_BV(_dado02) & ~_BV(_dado03);
			PORTB |= _BV(_enable_dez);
			PORTB |= dozen;
			display = 0;
			break;
	}
}

ISR(TIMER0_COMPA_vect)
{
	sinoaidal_phase_trigger++;
	if(sinoaidal_phase_trigger >= var){
		TIMSK0 &= ~(1<<OCIE0A);         // Disable timer0 compare interrupt
		PORTD |= _BV(PD4);              // Pulse moc trigger
		_delay_us(15);
		PORTD &= ~_BV(PD4);
		sinoaidal_phase_trigger = 0;
	}
}


ISR(INT0_vect)
{
    TIMSK0 = _BV(OCIE0A);                // Enable CTC interrupt for timer0
}

