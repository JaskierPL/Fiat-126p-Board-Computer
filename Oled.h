/*
 * Oled.h
 *
 * Created: 2015-04-07 20:32:17
 *  Author: Maciek
 */ 

#define H(port, pin) port |= _BV((pin))
#define L(port, pin) port &= ~(_BV((pin)))

#ifndef OLED_H_
#define OLED_H_

#include "OledConfig.h"

//Bufor
unsigned char oled_buffer[1024];

//Komunikacja z ekranem
void oled_write(uint8_t dc, uint8_t data);
void oled_init(void);
void oled_set_column(unsigned char column);
void oled_set_page(unsigned char page);

//Funkcje graficzne
void oled_set_pixel(unsigned char x, unsigned char y, unsigned char state, unsigned char inverted);
void oled_clear(void);
void oled_draw(void);

#endif