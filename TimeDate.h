#ifndef TIMEDATE_H
#define TIMEDATE_H

#include <avr/io.h>
#include <stdlib.h>

#include "LCD.h"
#include "Encoder.h"
#include "Menu.h"
#include "RTC.h"

uint8_t n_hour, n_minute, n_second, n_day, n_month, n_year;

unsigned char td_change_mode;
signed char td_selected_item;

extern unsigned char make_measurement;

void init_timedate(void);
void update_timedate(void);
void draw_timedate(void);

#endif