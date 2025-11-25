/*
 * dimer.c
 *
 * Created: 01/09/2016 23:00:26
 *  Author: Nestor
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define _chave02 6
#define _chave01 7

#define _dado00 2
#define _dado01 3
#define _dado02 4
#define _dado03 5
#define _enable_uni 1
#define _enable_dez 0


#define VALUE_MAX_HALF_CICLO	100
#define VALUE_MAX_ANGLE			VALUE_MAX_HALF_CICLO
#define VALUE_MIN_ANGLE			1

#define NUM_VEZES_LENTO			5
#define TEMPO_INCREMENTO_LENTO	600
#define TEMPO_INCREMENTO_RAPIDO 65

#define if_value_min(a)		if(var <= 1) var = a
#define if_value_max(a)		if(var >= 99) var = a

#define RTC_desliga		TIMSK &= ~(1 << TOIE2)
#define RTC_liga		TIMSK |= (1 << TOIE2)


unsigned int timeAngle = 0;

unsigned char segundos = 0, minutos = 30, horas = 12;
			 
char display = 0,
     div,
	 var=0,
	 unidade,
	 dezena;
			  
void convertVariavel(unsigned int v);



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
			 
ISR(INT0_vect)
{
	TIMSK0 = _BV(OCIE0A);				//Habilita a interrupção de CTC do timer/counter0
}

 ISR(TIMER0_COMPA_vect)
{
	timeAngle++;
	if(timeAngle >= var){
		TIMSK0 &= ~(1<<OCIE0A);			//Desabilita a interrupção de overflow do timer/counter0
		PORTD |= _BV(PD4);				//Pulso no ino de disparo do moc
		_delay_us(15);
		PORTD &= ~_BV(PD4);
		timeAngle = 0;					
	}
}


int main(void)
{
	unsigned int contTec = 0,
				 contTecRapido = 0;
	
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
	TCCR2 = (1 << CS22) | (1 << CS20);	
	
	//habilita a interrupção do timer 2
	TIMSK |= 1 << TOIE2;				

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



void convertVariavel(unsigned int v)
{
	unidade = v%10;
	div = v/10;
	dezena = div%10;
	dezena = dezena << 2;
	unidade = unidade << 2;
}