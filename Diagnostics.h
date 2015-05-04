#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <avr/io.h>
#include <stdlib.h>

#include "Display.h"
#include "Encoder.h"
#include "Menu.h"

float rtc_voltage;
char rtc_voltage_buffer[8];

void init_diagnostics(void);
void update_diagostics(void);
void draw_diagnostics(void);

#endif