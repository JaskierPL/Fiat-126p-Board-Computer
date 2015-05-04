#include "LCD.h"

/* Przesy³a bajt danych */
void lcd_write_byte(unsigned char c_d, unsigned char data)
{
	unsigned char m;

	LCD_CE_CLR

	if(c_d)
	LCD_DC_SET
	else
	LCD_DC_CLR

	for(m=0x80; m; m>>=1)
	{
		if(data & m)
		LCD_DATA_SET
		else
		LCD_DATA_CLR
		
		LCD_CLK_SET
		LCD_NOP
		LCD_CLK_CLR
	}

	LCD_CE_SET
}

/* Przesy³a komendê do wyœwietlacza */
void lcd_write_command(unsigned char command)
{
	lcd_write_byte(LCD_CMD, command);
}

/* Przesy³a dane do wyœwietlacza */
void lcd_write_data(unsigned char data)
{
	lcd_write_byte(LCD_DATA, data);
}

/* Inicjuje wyœwietlacz */
void lcd_init(void)
{
	lcd_clear();
	
	LCD_RST_CLR
	LCD_RST_SET_OUT
	LCD_CLK_SET_OUT
	LCD_DATA_SET_OUT
	LCD_DC_SET_OUT
	LCD_CE_SET_OUT

	LCD_RST_CLR
	LCD_RST_SET

	lcd_write_command(0x21); // Function set - extended instruction set
	lcd_write_command(0x14); // Bias - 1:48
	lcd_write_command(0x04); // Temperature Control
	lcd_write_command(0xb8); // Set Vop
	lcd_write_command(0x20); // Function set - basic instruction set, horizontal addressing
	lcd_write_command(0x0C); // Display control - normal mode
	
	lcd_draw();
}

/* Ustawia piksel w buforze */
void lcd_set_pixel(unsigned char x, unsigned char y, unsigned char state,  unsigned char inverted)
{
	if(x < 84 && y < 48)
	{
		int z = x + (y / 8) * 84;
		unsigned char b = y%8;
		
		if((state >= 1 && inverted == 0) || (state == 0 && inverted == 1))
			lcd_buffer[z] |= (1<<b);
		else
			lcd_buffer[z] &= ~(1<<b);
	}
}

/* Czyœci bufor */
void lcd_clear(void)
{
	for(int i = 0; i < 504; i++)
		lcd_buffer[i] = 0x00;	
}

/* Przesy³a dane bufora do ekranu */
void lcd_draw(void)
{
	lcd_write_command(0x20); //Adresowanie poziome
	lcd_write_command(0x40); //Y = 0
	lcd_write_command(0x80); //X = 0
	
	for(int i = 0; i < 504; i++)
		lcd_write_data(lcd_buffer[i]);
}
