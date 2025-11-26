#ifndef PERIPHERALS_CONFIG_H
#define PERIPHERALS_CONFIG_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include "dimmer_digital/dimmer_defs.h"

/**
 * @file
 * @brief Peripheral configuration APIs for the dimmer firmware.
 *
 * This header exposes small initialization helpers that configure the
 * microcontroller peripherals used by the project (timers, external
 * interrupt, display outputs, triac output and key inputs).
 *
 * Each function is intended to be called once from application startup
 * (typically from `main()`), and keeps initialization logic grouped and
 * testable.
 */

/**
 * @brief Configure all peripherals used by the application.
 *
 * This is a convenience function that can call the more specific
 * configuration routines below. It sets up timers, external interrupt,
 * I/O directions and any other hardware required before enabling global
 * interrupts.
 */
void configPeripherals(void);


/**
 * @brief Configure the external interrupt used for zero-cross detection.
 *
 * Configures the appropriate pin as input with pull-up (if required)
 * and selects the interrupt trigger (e.g. falling edge). It also
 * enables the external interrupt in the interrupt mask.
 */
void configIntExternal(void);


/**
 * @brief Configure the RTC timer (Timer/Counter2) for 1-second ticks.
 *
 * This configures the asynchronous timer/counter used as a simple RTC
 * source (if present). It sets the prescaler and enables the overflow
 * interrupt so the application can maintain wall-clock time.
 */
void configRtc(void);


/**
 * @brief Enable Timer/Counter2 overflow interrupt.
 *
 * Sets the appropriate interrupt mask bit so the timer overflow ISR
 * is called when Timer2 rolls over. The implementation should ensure
 * the correct TIMSK register is used for the target MCU.
 */
void configIntOverflowTimer2(void);


/**
 * @brief Enable Timer/Counter0 compare-match interrupt (CTC mode).
 *
 * Configures the output-compare interrupt for timer0. The function
 * may also select the prescaler and the CTC mode depending on the
 * project's requirements.
 */
void configIntCompareMatchTimer0(void);


/**
 * @brief Enable Timer/Counter1 compare-match interrupt.
 *
 * Typically used for multiplexing the 7-segment display or other
 * periodic tasks. This config function should set the compare value
 * and enable the OCIE1A interrupt.
 */
void configIntCompareMatchTimer1(void);


/**
 * @brief Configure the GPIO pins used by the 7-segment display.
 *
 * Sets the appropriate DDRx bits to outputs and ensures initial
 * output states are safe (for example, drivers disabled until the
 * display routine is ready).
 */
void configDisplayOutputs(void);


/**
 * @brief Configure the Triac trigger output pin.
 *
 * Sets the triac driver pin as output and initializes it to the
 * de-asserted state. The triac should be triggered only from a
 * controlled place in the code (usually from an ISR or from the
 * main loop after an event).
 */
void configTriacOutput(void);


/**
 * @brief Configure key (button) input pins with pull-ups.
 *
 * Sets the key pins as inputs and enables the internal pull-up
 * resistors so keys read as logical HIGH when released.
 */
void configKeysInputs(void);

#endif