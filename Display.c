#include "Display.h"

/* Czcionka */
const unsigned char font[] PROGMEM = {
	#include "font6x8p.h"
};

/* Wybór wyœwietlacza */
void display_set_display(unsigned char display_id)
{
	current_display = display_id;
}

/* Inicjacja wyœwietlacza */
void display_init_display(unsigned char display_id)
{
	switch(display_id)
	{
		case NOKIA_3310_DISPLAY:
			lcd_init();
			break;
		case SH1106_DISPLAY:
			oled_init();
			break;
	}
}

/* Ustawienie piksela */
void display_set_pixel(unsigned char x, unsigned char y, unsigned char state)
{
	switch(current_display)
	{
		case NOKIA_3310_DISPLAY:
			lcd_set_pixel(x, y, state, invert_mode);
			break;
		case SH1106_DISPLAY:
			oled_set_pixel(x, y, state, invert_mode);
			break;
	}
}

/* Wyœwietla prostok¹t */
void display_rectangle(unsigned char x, unsigned char y, unsigned char w, unsigned char h)
{
	for(unsigned char yt = 0; yt < h; yt++)
	for(unsigned char xt = 0; xt < w; xt++)
	display_set_pixel(x + xt, y + yt, 1);
}

/* Wyœwietla bitmapê */
void display_bitmap(const unsigned char image[], unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char size, unsigned char pgm)
{
	unsigned char rh = h/8;
	if(h%8 > 0) rh++;

	for(int a = 0; a < rh; a++)
	{
		for(int d = 0; d < size; d++)
		{
			for(int b = 0; b < w; b++)
			{
				for(int f = 0; f < size; f++)
				{
					int g = d*8/size;
					
					for(int i = 0; i < 8; i++)
					{
						if(i > 0 && i%size == 0) g++;
						
						if(pgm) display_set_pixel(x + b*size + f, y + a*8*size + d*8 + i, pgm_read_byte(&image[a*w + b]) & (1<<g));
						else display_set_pixel(x + b*size + f, y + a*8*size + d*8 + i, image[a*w + b] & (1<<g));
					}
				}
			}
		}
	}
}

/* Wyœwietla tekst */
void display_text(char text[], unsigned char x, unsigned char y, unsigned char size)
{
	unsigned char p;
	
	char pl[] = {'¹','æ','ê','³','ñ','ó','œ','Ÿ','¿','¥','Æ','Ê','£','Ñ','Ó','Œ','','¯', '°'};
	
	for(int i = 0; text[i] != '\0'; i++)
	{
		char c = text[i];
		
		// Polskie znaki
		for(p = 0; (p < 19) && (pl[p] != c); p++);
		if(p < 19) c = 0x80 + p;
		
		unsigned int n = (c - 32) * 6;
		unsigned char ch[6] = {pgm_read_byte(&font[n]),
			pgm_read_byte(&font[n+1]),
			pgm_read_byte(&font[n+2]),
			pgm_read_byte(&font[n+3]),
			pgm_read_byte(&font[n+4]),
		pgm_read_byte(&font[n+5])};
		
		display_bitmap(ch, x + i*6*size, y, 6, 8, size, 0x00);
	}
}

/* Wyœwietla tekst z doadatkowymi ustawieniami */
void display_custom_text(char text[], unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char halign, unsigned char valign, unsigned char size)
{
	unsigned char cx = x;
	unsigned char cy = y;

	if(halign == 1) cx += w / 2 - (strlen(text) * 6 * size) / 2;
	else if(halign == 2) cx += w - strlen(text) * 6 * size;
	
	if(valign == 1) cy += h / 2 - 6 * size / 2;
	else if(valign == 2) cy += h - 6 * size;

	display_text(text, cx, cy, size);
}

/* Czyœci bufor */
void display_clear(void)
{
	switch(current_display)
	{
		case NOKIA_3310_DISPLAY:
			lcd_clear();
			break;
		case SH1106_DISPLAY:
			oled_clear();
			break;
	}	
}

/* Rysuje obraz */
void display_draw(void)
{
	switch(current_display)
	{
		case NOKIA_3310_DISPLAY:
			lcd_draw();
			break;
		case SH1106_DISPLAY:
			oled_draw();
			break;
	}
}