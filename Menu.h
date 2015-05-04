#ifndef MENU_H
#define MENU_H

#include <avr/io.h>

#include "Display.h"
#include "Encoder.h"

#include "Diagnostics.h"
#include "TimeDate.h"

extern char state;

signed char menu_selected;
unsigned char screen_on;
unsigned char screen_mode;

void update_menu(void);
void draw_menu(void);

#endif