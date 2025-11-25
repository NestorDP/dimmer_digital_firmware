#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "dimmer_digital/dimmer_defs.h"

ISR(TIMER2_OVF_vect)
{
    segundos++;
    if(segundos == 60){
        segundos = 0;
        minutos++;
        if(minutos == 60){
            minutos = 0;
            horas++;
            if(horas == 24){
                horas = 0;
            }
        }
    }
}

ISR(TIMER1_COMPA_vect)
{
    switch(display)
    {
        case 0:
            PORTB &= ~_BV(_enable_dez);
            PORTB &= ~_BV(_dado00) & ~_BV(_dado01) & ~_BV(_dado02) & ~_BV(_dado03);
            PORTB |= _BV(_enable_uni);
            PORTB |= unidade;
            display = 1;
            break;
        case 1:
            PORTB &= ~_BV(_enable_uni);
            PORTB &= ~_BV(_dado00) & ~_BV(_dado01) & ~_BV(_dado02) & ~_BV(_dado03);
            PORTB |= _BV(_enable_dez);
            PORTB |= dezena;
            display = 0;
            break;
    }
}

ISR(TIMER0_COMPA_vect)
{
    timeAngle++;
    if(timeAngle >= var){
        TIMSK0 &= ~(1<<OCIE0A);         // Disable timer0 compare interrupt
        PORTD |= _BV(PD4);              // Pulse moc trigger
        _delay_us(15);
        PORTD &= ~_BV(PD4);
        timeAngle = 0;
    }
}


ISR(INT0_vect)
{
    TIMSK0 = _BV(OCIE0A);                // Enable CTC interrupt for timer0
}

