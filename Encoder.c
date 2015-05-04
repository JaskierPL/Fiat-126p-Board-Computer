#include "Encoder.h"

void encoder_init(void)
{
	DDRA |= (1 << PA1) | (1 << PA2); //Kana³y A i B
	DDRA |= (1 << PA3); //Przycisk
	PORTA |= (1 << PA1) | (1 << PA2) | (1 << PA3); //Pull-up
	
	encoder_btn_down = 0;
	encoder_btn_pressed = 0;
	encoder_channel_A_prev = 0;
	encoder_rot = 0;
}

unsigned char is_encoder_btn_down()
{
	return encoder_btn_down;
}

unsigned char is_encoder_btn_pressed()
{
	if(encoder_btn_pressed == 1)
	{
		encoder_btn_pressed = 0;
		return 1;
	}
	else return 0;
}

signed char get_encoder_rot(void)
{
	signed char rot = encoder_rot;
	
	encoder_rot = 0;
	
	return rot;
}

void encoder_update(void)
{
	//Przycisk
	if(bit_is_clear(PINA, 3))
	{
		if(encoder_btn_down == 0)
		encoder_btn_pressed = 1;
		
		encoder_btn_down = 1;
	}
	else
	encoder_btn_down = 0;
	
	//Pokrêt³o
	encoder_channel_A = bit_is_clear(PINA, 1);
	encoder_channel_B = bit_is_clear(PINA, 2);
	
	if(encoder_channel_A == 1 && encoder_channel_A_prev == 0)
	{
		if(encoder_channel_B)
		{
			if(encoder_rot < 127) encoder_rot ++;
		}
		else
		{
			if(encoder_rot > -127) encoder_rot --;
		}
	}
	
	encoder_channel_A_prev = encoder_channel_A;
}