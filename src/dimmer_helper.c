
#include "dimmer_digital/dimmer_helper.h"
#include "dimmer_digital/dimmer_defs.h"

void convertVariable(unsigned int v)
{
	unit = v%10;
	divis = v/10;
	dozen = divis%10;
	dozen = dozen << 2;
	unit = unit << 2;
}