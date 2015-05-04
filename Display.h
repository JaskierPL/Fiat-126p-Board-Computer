#include "LCD.h" //Nokia 3310
#include "Oled.h" //SH1106

#define NOKIA_3310_DISPLAY 0
#define SH1106_DISPLAY 1

#ifndef DISPLAY_H_
#define DISPLAY_H_

int current_display;
unsigned char invert_mode;

void display_set_display(unsigned char display_id);
void display_init_display(unsigned char display_id);

//Funkcje graficzne
void display_set_pixel(unsigned char x, unsigned char y, unsigned char state);
void display_rectangle(unsigned char x, unsigned char y, unsigned char w, unsigned char h);
void display_bitmap(const unsigned char image[], unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char size, unsigned char pgm);
void display_text(char text[], unsigned char x, unsigned char y, unsigned char size);
void display_custom_text(char text[], unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char halign, unsigned char valign, unsigned char size);
void display_clear(void);
void display_draw(void);

#endif