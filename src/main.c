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
	unsigned int cont_tec = 0;
  unsigned int cont_tec_rapido = 0;

	configPeripherals();	
	convertVariable(var);
	
	while (1)
	{ 
    //Testa se a chave 01 foi precionada (ch++)	
		if (bit_is_clear(PIND, _chave01))							
		{
      //espera passar o bounce da chave
			_delay_ms(20);
      
      //incrementa variável do display
			var++;

      //atribue um valor máximo para a variável
			if_value_max(99);

      //quebra a variável em unit e dozen para o display 7 segmentos
			convertVariable(var);

      //enquanto a chave tiver precionada incrementa a variável
			while(bit_is_clear(PIND, _chave01)) {
        //testa quantas vezes a var foi incrementada sem ser solta
				if(cont_tec_rapido < NUM_VEZES_LENTO) {	
          //testa se já passo o tempo para incrmento lento	
					if(cont_tec >= TEMPO_INCREMENTO_LENTO) {	
						var++;					
						if_value_max(99);
						convertVariable(var);

            //incrementa a variável para mudança de lento para rápido
						cont_tec_rapido++;

            //zera variável do tempo do teclado
						cont_tec = 0;
					}
				}
        //se passar o numer maximas de contagens lentas
				else{
          //testa se já passou o tempo para incremento rápido
					if(cont_tec >= TEMPO_INCREMENTO_RAPIDO){
						var++;
						if_value_max(99);
						convertVariable(var);

            //zera variável do tempo do teclado
						cont_tec = 0;
					}
				}
        //espera um ms para as contagens de tempo entre os incrementos
				_delay_ms(1);

        //incrementa a variável que guarda o tempo passado para o incremento
				cont_tec++;
			}
      //reinicia a variável para mudança de lento para rápido
			cont_tec_rapido = 0;

      //reinicia a variável para contar o tempo dos incremento do teclado
			cont_tec = 0;
		}
		if (bit_is_clear(PIND, _chave02))
		{
			_delay_ms(20);
			if_value_min(1);
			var--;
			convertVariable(var);
			while(bit_is_clear(PIND, _chave02))
			{
				if(cont_tec_rapido < NUM_VEZES_LENTO){
					if(cont_tec >= TEMPO_INCREMENTO_LENTO){
						if_value_min(1);
						var--;
						convertVariable(var);

            //incrementa a variável para mudança de lento para rápido
						cont_tec_rapido++;

            //zera variável do tempo do teclado
						cont_tec = 0;
					}
				}
				else{
					if(cont_tec >= TEMPO_INCREMENTO_RAPIDO){
						if_value_min(1);
						var--;
						convertVariable(var);
            
            //zera variável do tempo do teclado
						cont_tec = 0;
					}
				}
				_delay_ms(1);
				cont_tec++;
			}

      //reinicia a variável para mudança de lento para rápido
			cont_tec_rapido = 0;
      
      //reinicia a variável para contar o tempo dos incremento do teclado
			cont_tec = 0;
		}
	}
}
