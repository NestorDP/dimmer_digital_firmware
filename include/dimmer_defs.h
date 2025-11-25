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