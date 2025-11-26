#include "dimmer_digital/peripherals_config.h"

void configPeripherals(void)
{
  configIntExternal();
  configRtc();
  configIntOverflowTimer2();
  configIntCompareMatchTimer0();
  configIntCompareMatchTimer1();
  configDisplayOutputs();
  configTriacOutput();
  configKeysInputs();
  // Enable all interrupts
	sei();
}

void configIntExternal(void)
{
  // Set up the PortD.2 as input with the pull-up resistor enabled
	// This pin will serve as input for the external interrupt of the zero detector
	DDRD &= ~_BV(PD2);
	PORTD |= _BV(PD2);
		
	// Enable external interrupt 0
	// EIMSK – External Interrupt Mask Register
	// Bit 0 – INT0: External Interrupt Request 0 Enable
	EIMSK |= _BV(INT0);
	
	// Configure interrupt on falling edge
	// EICRA – External Interrupt Control Register A
	// Bit 1 and Bit 0 – ISC01 and ISC00: Interrupt Sense Control 0
	EICRA |= _BV(ISC01);
}

void configRtc(void){
  // Enable external crystal for the RTC with 32.768kHz crystal
	// ASSR – Asynchronous Status Register
	// Bit 5 – AS2: Asynchronous Timer/Counter2
	ASSR = (1 << AS2);
}

void configIntOverflowTimer2(void)
{
	// prescaler = 128
	// clk = 32.768kHz crystal (RTC enabled)
	// clk/128 = 256Hz
	// 1/256Hz = 3.90625ms per increment
	// 256 * 3.90625ms = 1s
	TCCR2A = (1 << CS22) | (1 << CS20);	

	// Enable timer 2 overflow interrupt
	// TIMSK2 – Timer/Counter2 Interrupt Mask Register
	// Bit 0 – TOIE2: Timer/Counter2 Overflow Interrupt Enable
	TIMSK2 |= _BV(TOIE2);		
}

void configIntCompareMatchTimer0(void)
{
  // Timer/counter 0 prescale 8
	// TCCR0B – Timer/Counter Control Register B
	// Bits 2:0 – CS02:0: Clock Select
	// clk = 8000000Hz
	// clk/8 = 1000000Hz
	// 1/1000000Hz = 1us per increment
	TCCR0B |= _BV(CS01);

	// Clear timer on compare match (CTC) mode
	// TCCR0A – Timer/Counter Control Register A
	// Bits 1:0 – WGM01:0: Waveform Generation Mode (WGM02 = 0 in TCCR0B)
	TCCR0A |= _BV(WGM01);
	
	// initialize counter
	//  TCNT0 – Timer/Counter Register 0
	TCNT0 = 0;
	
	// Load the count value into the Output Compare Register 0 A
	// OCR0A – Output Compare Register A 0
	OCR0A = 79; // 80us
}

void configIntCompareMatchTimer1(void)
{
  // timer/counter 1 prescale 8
	// TCCR1B – Timer/Counter1 Control Register B
	// Bits 2:0 – CS12:0: Clock Select
	TCCR1B |= _BV(CS11);
	
	// Clear Timer on Compare CTC mode
	// TCCR1B – Timer/Counter1 Control Register B
	// Bit 4:3 – WGM13:2: Waveform Generation Mode (WGM11 = 0 and WGM10 = 0 in TCCR1A)
	TCCR1B |= _BV(WGM12);
	
	// initialize counter
	// TCNT1 – Timer/Counter1 Register
	TCNT1 = 0;
	
	// Load the count value into the Output Compare Register 1 A
	// OCR1A – Output Compare Register A 1
	OCR1A = 999;
	
	// Enable the timer/counter 1 interrupt
	// TIMSK1 – Timer/Counter1 Interrupt Mask Register
	// Bit 1 – OCIE1A: Timer/Counter1 Output Compare A Match Interrupt Enable
	TIMSK1 = _BV(OCIE1A);
}

void configDisplayOutputs(void)
{
  // Set up seven segment display outputs
  DDRB |= _BV(PB0) | _BV(PB1) | _BV(PB2) | _BV(PB3) | _BV(PB4) | _BV(PB5);
}

void configTriacOutput(void)
{
  // Set up output for Triac trigger
  DDRD |= _BV(PD4);
}

void configKeysInputs(void)
{
  // Set up inputs for keys with pull-up resistors
	DDRD &= ~_BV(_chave01) & ~_BV(_chave02);
	PORTD |= _BV(_chave01) | _BV(_chave02);
}

