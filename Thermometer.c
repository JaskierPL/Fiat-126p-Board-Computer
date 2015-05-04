#include "Thermometer.h"

unsigned char therm_reset_pulse(void)
{
	THERM_DATA_CLR;
	THERM_DATA_SET_OUT;
	_delay_us(480);
	THERM_DATA_SET_IN;
	_delay_us(60);
	
	if(bit_is_clear(PINB, 0))
		therm_presence = 1;
	else 
		therm_presence = 0;
		
	_delay_us(420);
	
	return therm_presence;
}

void therm_send_bit(char c_bit)
{
	THERM_DATA_CLR;
	THERM_DATA_SET_OUT;
	_delay_us(1);
	
	if(c_bit == 1)
		THERM_DATA_SET_IN;
	
	_delay_us(60);
	THERM_DATA_SET_IN;
}

unsigned char therm_read_bit(void)
{
	unsigned char c_bit = 0;
	
	THERM_DATA_CLR;
	THERM_DATA_SET_OUT;
	_delay_us(1);
	THERM_DATA_SET_IN;
	_delay_us(14);
	
	if(bit_is_set(PINB, 0))
		c_bit = 1;
	else 
		c_bit = 0;
		
	_delay_us(45);
	
	return c_bit;
}

void therm_send_byte(char c_byte)
{
	unsigned char ucValueToSend;
	
	for(unsigned char i = 0; i < 8; i++)
	{
		ucValueToSend = c_byte >> i;
		ucValueToSend &= 0x01;
		therm_send_bit(ucValueToSend);
	}
	
}

unsigned char therm_read_byte(void)
{
	unsigned char ucCounter;
	unsigned char ucReadByte = 0;
		
	for(ucCounter = 0; ucCounter < 8; ucCounter++)
	{
		if(therm_read_bit())
			ucReadByte |= 0x01<<ucCounter;	
	}
	
	return ucReadByte;
}

char therm_measure_begin(void)
{	
	if(!measurement_in_process)
	{
		if(therm_reset_pulse() == 1)
		{
			therm_send_byte(0xCC); //SKIP ROM
			therm_send_byte(0x44); //CONVERT T
		
			measurement_in_process = 1;
		
			return 1;
		}
		else
			return 0;
	}
	else return 0;
}

char therm_measure_fetch_results(void)
{
	char therm_temp_h = 0;
	char therm_temp_l = 0;
	therm_temp_f = 0;
		
	if(therm_read_bit())
	{
		therm_reset_pulse(); //Impuls reset
		therm_send_byte(0xCC); //SKIP ROM
		therm_send_byte(0xBE); //READ SCRATCHPAD
		therm_temp_l = therm_read_byte(); //Odczytanie LSB
		therm_temp_h = therm_read_byte(); //Odczytanie MSB
		therm_reset_pulse(); //Impuls reset
			
		therm_temp_f = (float)(therm_temp_l + (therm_temp_h << 8)) / 16;
		dtostrf(therm_temp_f, 1, 1, therm_string_buffer);
			
		measurement_in_process = 0;
			
		return 1;
	}
	else return 0;
}