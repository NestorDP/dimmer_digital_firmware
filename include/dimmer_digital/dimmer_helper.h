#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @file
 * @brief Helper utilities for the dimmer firmware.
 *
 * This header exposes small helper routines used by the application
 * (for example, value conversion routines used by the display code).
 * Keep these helpers small and portable; avoid hardware-specific side
 * effects in this layer when possible.
 */

/**
 * @brief Convert a numeric variable into display-friendly components.
 *
 * The exact behaviour depends on the implementation: typically this
 * function splits the provided value into digits (units, tens, ...)
 * and encodes or shifts them so they can be written directly to the
 * display port. Preferably the function should not perform I/O itself
 * but prepare values for the display driver.
 *
 * @param v The numeric value to convert (e.g. 0..99).
 *
 * @note Implementations that update shared state used by ISRs should
 * declare those variables as `volatile` and ensure proper atomic
 * access when multi-byte values are involved.
 */
void convertVariable(unsigned int v);

#endif // HELPERS_H