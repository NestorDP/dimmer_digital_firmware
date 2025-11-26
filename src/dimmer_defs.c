#include "dimmer_digital/dimmer_defs.h"

unsigned int sinoaidal_phase_trigger = 0;
unsigned char seconds = 0;
unsigned char minutes = 30;
unsigned char hours = 12;

char display = 0;
char unit = 0;
char dozen = 0;
char dimmer_value_percent = 50; // initial dimmer value