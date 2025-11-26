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
#include "dimmer_digital/peripherals_config.h"

int main(void)
{
	unsigned int contTec = 0;
  unsigned int contTecRapido = 0;

	configPeripherals();	
	convertVariable(var);
	
	while (1)
	{ 
		if (bit_is_clear(PIND, _chave01))					//Testa se a chave 01 foi precionada (ch++)			
		{
			_delay_ms(20);									//espera passar o bounce 
			var++;											//incrementa variável do display
			if_value_max(99);								//atribue um valor máximo para a variável
			convertVariable(var);							//quebra a variável em unit e dozen para o display 7 seg
			while(bit_is_clear(PIND, _chave01))				//enquanto a chave tiver precionada incrementa a variável
			{
				if(contTecRapido < NUM_VEZES_LENTO){		//testa quantas vezes a var foi incrementada sem ser solta
					if(contTec >= TEMPO_INCREMENTO_LENTO){	//testa se já passo o tempo para incrmento lento
						var++;					
						if_value_max(99);
						convertVariable(var);
						contTecRapido++;					//incrementa a variável para mudança de lento para rápido
						contTec = 0;						//zera variável do tempo do teclado
					}
				}
				else{										//se passar o numer maximas de contagens lentas
					if(contTec >= TEMPO_INCREMENTO_RAPIDO){	//testa se já passou o tempo para incremento rápido
						var++;
						if_value_max(99);
						convertVariable(var);
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
			convertVariable(var);
			while(bit_is_clear(PIND, _chave02))
			{
				if(contTecRapido < NUM_VEZES_LENTO){
					if(contTec >= TEMPO_INCREMENTO_LENTO){
						if_value_min(1);
						var--;
						convertVariable(var);
						contTecRapido++;					//incrementa a variável para mudança de lento para rápido
						contTec = 0;						//zera variável do tempo do teclado
					}
				}
				else{
					if(contTec >= TEMPO_INCREMENTO_RAPIDO){
						if_value_min(1);
						var--;
						convertVariable(var);
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
