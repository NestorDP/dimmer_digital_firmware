/*
 * dimer.c
 *
 * Created: 01/09/2016 23:00:26
 *  Author: Nestor
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "dimmer_digital/dimmer_defs.h"
#include "dimmer_digital/dimmer_helper.h"

int main(void)
{
	unsigned int contTec = 0;
  unsigned int contTecRapido = 0;
	
	//////////////////////////////////////////////////////////////////////////////////
	//configura o PortD.2 como entrada com o resistor de pull-up ativo
	//esse pino servirá de entrada para a interrupção externa do detectr de zero
	DDRD &= ~_BV(PD2);
	PORTD |= _BV(PD2);
		
	//Habilita a interrupção externa 0 
	EIMSK |= _BV(INT0);
	
	//Configura interrupção por borda de descida
	EICRA |= _BV(ISC01);
	
	//////////////////////////////////////////////////////////////////////////////////
	//Habilita o cristal externo para o RTC
	ASSR = 1 << AS2;
	
	//prescaler = 128, resultando em um estouro a cada 1s					
	TCCR2A = (1 << CS22) | (1 << CS20);	
	
	//habilita a interrupção do timer 2
	TIMSK0 |= 1 << TOIE2;				

	//////////////////////////////////////////////////////////////////////////////////
	//prescale do timer/counter0 8
	TCCR0B |= _BV(CS01);
	
	//modo de Clear Timer on Compare CTC
	TCCR0A |= _BV(WGM01);
	
	// initialize counter
	TCNT0 = 0;
	
	//Carrega o valor da contagem no registrado  Output Compare Register 1 A
	OCR0A = 79;
	
	
	//////////////////////////////////////////////////////////////////////////////////
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
	
	//////////////////////////////////////////////////////////////////////////////////
	//Habilita todas as interrupções
	sei();
	
	//////////////////////////////////////////////////////////////////////////////////
	//Definindo as saidas do display sete seguimentos
	DDRB |= _BV(PB0) | _BV(PB1) | _BV(PB2) | _BV(PB3) | _BV(PB4) | _BV(PB5);
	
	//Definido saída do disparo do Triac
	DDRD |= _BV(PD4);
	
	//Definindo entradas para as chaves com pull-up
	DDRD &= ~_BV(_chave01) & ~_BV(_chave02);
	PORTD |= _BV(_chave01) | _BV(_chave02);
	
	convertVariavel(var);
	
	while (1)
	{ 
		if (bit_is_clear(PIND, _chave01))					//Testa se a chave 01 foi precionada (ch++)			
		{
			_delay_ms(20);									//espera passar o bounce 
			var++;											//incrementa variável do display
			if_value_max(99);								//atribue um valor máximo para a variável
			convertVariavel(var);							//quebra a variável em unidade e dezena para o display 7 seg
			while(bit_is_clear(PIND, _chave01))				//enquanto a chave tiver precionada incrementa a variável
			{
				if(contTecRapido < NUM_VEZES_LENTO){		//testa quantas vezes a var foi incrementada sem ser solta
					if(contTec >= TEMPO_INCREMENTO_LENTO){	//testa se já passo o tempo para incrmento lento
						var++;					
						if_value_max(99);
						convertVariavel(var);
						contTecRapido++;					//incrementa a variável para mudança de lento para rápido
						contTec = 0;						//zera variável do tempo do teclado
					}
				}
				else{										//se passar o numer maximas de contagens lentas
					if(contTec >= TEMPO_INCREMENTO_RAPIDO){	//testa se já passou o tempo para incremento rápido
						var++;
						if_value_max(99);
						convertVariavel(var);
						contTec = 0;						//zera variável do tempo do teclado
					}
				}
				_delay_ms(1);								//espera um ms para as contagens de tempo entre os incrementos
				contTec++;									//incrementa a variável que guarda o tempo passado para o incremento
			}
			contTecRapido = 0;								//reinicia a variável para mudança de lento para rápido
			contTec = 0;									//reinicia a variável para contar o tempo dos incremento do teclado	
		}
		if (bit_is_clear(PIND, _chave02))
		{
			_delay_ms(20);
			if_value_min(1);
			var--;
			convertVariavel(var);
			while(bit_is_clear(PIND, _chave02))
			{
				if(contTecRapido < NUM_VEZES_LENTO){
					if(contTec >= TEMPO_INCREMENTO_LENTO){
						if_value_min(1);
						var--;
						convertVariavel(var);
						contTecRapido++;					//incrementa a variável para mudança de lento para rápido
						contTec = 0;						//zera variável do tempo do teclado
					}
				}
				else{
					if(contTec >= TEMPO_INCREMENTO_RAPIDO){
						if_value_min(1);
						var--;
						convertVariavel(var);
						contTec = 0;						//zera variável do tempo do teclado
					}
				}
				_delay_ms(1);
				contTec++;
			}
			contTecRapido = 0;								//reinicia a variável para mudança de lento para rápido
			contTec = 0;
		}
	}
}
