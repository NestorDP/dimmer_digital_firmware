
#include "dimmer_digital/dimmer_helper.h"
#include "dimmer_digital/dimmer_defs.h"

void convertVariable(unsigned int v)
{
	unidade = v%10;
	divis = v/10;
	dezena = divis%10;
	dezena = dezena << 2;
	unidade = unidade << 2;
}