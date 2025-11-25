#ifndef PERIPHERALS_CONFIG_H
#define PERIPHERALS_CONFIG_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include "dimmer_digital/dimmer_defs.h"

void configPeripherals(void);

void configIntExternal(void);

void configRtc(void);

void configIntOverflowTimer2(void);

void configIntCompareMatchTimer0(void);

void configIntCompareMatchTimer1(void);

void configDisplayOutputs(void);

void configTriacOutput(void);

void configKeysInputs(void);

#endif