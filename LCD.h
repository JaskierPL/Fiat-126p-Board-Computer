#ifndef LCD_H
#define LCD_H

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <string.h>

// CLK
#define LCD_CLK_SET       PORTD |=  (1<<PD6);
#define LCD_CLK_CLR       PORTD &= ~(1<<PD6);
#define LCD_CLK_SET_OUT   DDRD  |=  (1<<PD6);

// DATA
#define LCD_DATA_SET      PORTD |=  (1<<PD5);
#define LCD_DATA_CLR      PORTD &= ~(1<<PD5);
#define LCD_DATA_SET_OUT  DDRD  |=  (1<<PD5);

// DC
#define LCD_DC_SET        PORTD |=  (1<<PD4);
#define LCD_DC_CLR        PORTD &= ~(1<<PD4);
#define LCD_DC_SET_OUT    DDRD  |=  (1<<PD4);

// CE
#define LCD_CE_SET        PORTD |=  (1<<PD3);
#define LCD_CE_CLR        PORTD &= ~(1<<PD3);
#define LCD_CE_SET_OUT    DDRD  |=  (1<<PD3);

// RST
#define LCD_RST_SET       PORTD |=  (1<<PD2);
#define LCD_RST_CLR       PORTD &= ~(1<<PD2);
#define LCD_RST_SET_OUT   DDRD  |=  (1<<PD2);

// NOP, CMD, DATA
#define LCD_NOP asm volatile("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" ::);
#define LCD_CMD     0
#define LCD_DATA    1

//Bufor
unsigned char lcd_buffer[504];

//Komunikacja z ekranem
void lcd_write_byte(unsigned char c_d, unsigned char data);
void lcd_write_command(unsigned char command);
void lcd_write_data(unsigned char data);
void lcd_init(void);

//Funkcje graficzne
void lcd_set_pixel(unsigned char x, unsigned char y, unsigned char state, unsigned char inverted);
void lcd_clear(void);
void lcd_draw(void);

#endif