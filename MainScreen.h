#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <avr/io.h>

#include "Thermometer.h"
#include "RTC.h"
#include "Encoder.h"
#include "Display.h"
#include "RTC.h"

extern char state;

uint8_t hour, minute, second, day, month, year;

//Flaga pomiaru
unsigned char make_measurement;

void update_main_screen(void);
void draw_main_screen(void);

#endif