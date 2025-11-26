
#include "dimmer_digital/dimmer_helper.h"
#include "dimmer_digital/dimmer_defs.h"

void convertVariable(unsigned int v)
{
	// Split the value into unit for 7-segment display
	unit = v%10;

	// Split the value into dozen for 7-segment display
	dozen = v/10;

	// Shift bits to match the display encoding with the port bits
	// assuming display connected to PORTB2..PORTB5
	unit = unit << 2;
	dozen = dozen << 2;
}