/*
 * Oled.c
 *
 * Created: 2015-04-07 20:32:31
 *  Author: Maciek
 * SH1106
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "OledConfig.h"
#include "Oled.h"

void oled_write(uint8_t dc, uint8_t data) 
{
	if(dc > 0) H(PORTC, SS1306_OLED_DC);
	else L(PORTC, SS1306_OLED_DC);
	
	L(PORTC, SS1306_OLED_CS);
	for(int8_t bit = 7; bit >= 0; bit--)
	{
		L(PORTC, SS1306_OLED_CLK);
		if((1 << bit) & data) H(PORTC, SS1306_OLED_DAT);
		else L(PORTC, SS1306_OLED_DAT);
		H(PORTC, SS1306_OLED_CLK);
	}
	L(PORTC, SS1306_OLED_CLK);
	H(PORTC, SS1306_OLED_CS);
}

void oled_init(void) 
{	
	DDRC |= SS1306_OLED_PORTC_MASK;
	
	L(PORTC, SS1306_OLED_RST);
	_delay_ms(10);
	H(PORTC, SS1306_OLED_RST);
	_delay_ms(10);
	
	oled_write(0, 0xAE); // display off
	
	oled_write(0, 0x00); //Low column
	oled_write(0, 0x10); //High column
	oled_write(0, 0xB0); //Page
	oled_write(0, 0x40); //Start line
	
	oled_write(0, 0xA1); // segment remap
	
	oled_write(0, 0xDA); // com hardware cfg
	oled_write(0, 0x12); // alt com cfg
	
	oled_write(0, 0xD3); // display offset
	oled_write(0, 0x00); // rtfm
	
	oled_write(0, 0xC0); // com scan direction
	oled_write(0, 0xC8); // com scan direction
	
	oled_write(0, 0xA6); // non-inverted
	oled_write(0, 0xA4); // display scan on
	
	oled_write(0, 0x81); // contrast aka current
	oled_write(0, 0x50); // 128 is midpoint
	
	oled_write(0, 0xA8); // mux ratio
	oled_write(0, 0x3F); // rtfm
	
	oled_write(0, 0xD5); // clock
	oled_write(0, 0x80); // upper nibble is rate, lower nibble is divisor
	
	oled_write(0, 0xD9); // precharge
	oled_write(0, 0xF1); // rtfm
	
	oled_write(0, 0xDB); // vcomh deselect level
	oled_write(0, 0x40); // rtfm
	
	oled_write(0, 0x8D); // charge pump
	oled_write(0, 0x14); // enable
	
	oled_write(0, 0xAF); // drivers on
}

void oled_set_column(unsigned char column)
{
	if(column > 128) return;
	++column;
	++column;
	oled_write(0, 0x00 | (column & 0x0F));
	oled_write(0, 0x10 | (column >> 4));
	return;
}

void oled_set_page(unsigned char page)
{
	if(page > 7) return;
	oled_write(0, 0xB0 | page);
	return;
}

/* Ustawia piksel w buforze */
void oled_set_pixel(unsigned char x, unsigned char y, unsigned char state, unsigned char inverted)
{
	if(x < 128 && y < 64)
	{
		int z = x + (y / 8) * 128;
		unsigned char b = y%8;
		
		if((state >= 1 && inverted == 0) || (state == 0 && inverted == 1))
			oled_buffer[z] |= (1<<b);
		else
			oled_buffer[z] &= ~(1<<b);
	}
}

void oled_clear()
{
	for(int i = 0; i < 1024; i++)
	{
		oled_buffer[i] = 0x00;
	}
}

void oled_draw(void)
{
	oled_set_column(0);
	oled_set_page(0);
	for(int i = 0; i < 1024; i++)
	{
		if(i % 128 == 0) 
		{
			oled_set_column(0);
			oled_set_page((unsigned char)(i / 128));
		}
		
		oled_write(1, oled_buffer[i]);
	}
	
}