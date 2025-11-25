#include "dimmer_digital/peripherals_config.h"

void configPeripherals(void){
  configIntExternal();
  configRtc();
  configIntOverflowTimer2();
  configIntCompareMatchTimer0();
  configIntCompareMatchTimer1();
  configDisplayOutputs();
  configTriacOutput();
  configKeysInputs();
  //Habilita todas as interrupções
	sei();
}

void configIntExternal(void){
  //configura o PortD.2 como entrada com o resistor de pull-up ativo
	//esse pino servirá de entrada para a interrupção externa do detectr de zero
	DDRD &= ~_BV(PD2);
	PORTD |= _BV(PD2);
		
	//Habilita a interrupção externa 0 
	EIMSK |= _BV(INT0);
	
	//Configura interrupção por borda de descida
	EICRA |= _BV(ISC01);
}

void configRtc(void){
  //Habilita o cristal externo para o RTC
	ASSR = 1 << AS2;
}

void configIntOverflowTimer2(void){
  //prescaler = 128, resultando em um estouro a cada 1s					
	TCCR2A = (1 << CS22) | (1 << CS20);	
	
	//habilita a interrupção do timer 2
	TIMSK0 |= 1 << TOIE2;		
}

void configIntCompareMatchTimer0(void){
  //prescale do timer/counter0 8
	TCCR0B |= _BV(CS01);
	
	//modo de Clear Timer on Compare CTC
	TCCR0A |= _BV(WGM01);
	
	// initialize counter
	TCNT0 = 0;
	
	//Carrega o valor da contagem no registrado  Output Compare Register 1 A
	OCR0A = 79;
}

void configIntCompareMatchTimer1(void){
  //Prescale do timer/counter1 8
	TCCR1B |= _BV(CS11);
	
	//modo de Clear Timer on Compare CTC
	TCCR1B |= _BV(WGM12);
	
	// initialize counter
	TCNT1 = 0;
	
	//Carrega o valor da contagem no registrado  Output Compare Register 1 A
	OCR1A = 999;
	
	//Habilita a interrupção do timer/counter 1
	TIMSK1 = _BV(OCIE1A);
}

void configDisplayOutputs(void){
  //Definindo as saidas do display sete seguimentos
  DDRB |= _BV(PB0) | _BV(PB1) | _BV(PB2) | _BV(PB3) | _BV(PB4) | _BV(PB5);
}

void configTriacOutput(void){
  //Definido saída do disparo do Triac
  DDRD |= _BV(PD4);
}

void configKeysInputs(void){
  //Definindo entradas para as chaves com pull-up
	DDRD &= ~_BV(_chave01) & ~_BV(_chave02);
	PORTD |= _BV(_chave01) | _BV(_chave02);
}

