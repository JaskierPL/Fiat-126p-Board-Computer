//Firmware komputera pok³adowego Fiat 126p

#define F_CPU 16000000UL

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

//Stan programu
char state = 0;

#include "Thermometer.h"
#include "Encoder.h"
#include "RTC.h"

#include "Display.h"

#include "MainScreen.h"
#include "Menu.h"

//Obrazek Intro
const unsigned char img[] PROGMEM = {
	#include "logo.h"
};

//Przepe³nienia licznika
unsigned int measurement_overflows = 0;
unsigned int encoder_btn_overflows = 0;

//Liczniki przepe³nieñ
unsigned int counter_btn = 0;
unsigned int counter_rot = 0;

int main(void)
{
	//Inicjalizacja wyœwietlaczy
	display_init_display(NOKIA_3310_DISPLAY);
	display_init_display(SH1106_DISPLAY);
	
	//Ekran powitalny na SH1106
	display_set_display(SH1106_DISPLAY);
	display_clear();
	display_bitmap(img, 0, 4, 84, 17, 1, 1);
	display_custom_text("126p", 0, 28, 84, 8, 1, 0, 2);
	display_draw();
	
	//Ekran powitalny na NOKIA 3310
	display_set_display(NOKIA_3310_DISPLAY);
	display_clear();
	display_bitmap(img, 0, 4, 84, 17, 1, 1);
	display_custom_text("126p", 0, 28, 84, 8, 1, 0, 2);
	display_draw();

	//Inicjalizacja Timera 0
	TCNT0 = 0;
	TCCR0 |= (1 << CS01); //Pre 8
	TIMSK |= (1 << TOIE0); //Przerwanie po przepe³nieniu
	
	//Inicjalizacja enkodera
	encoder_init(); 
	
	//Pomiar temperatury
	therm_measure_begin();

	//Zegar czasu rzeczywistego
	I2C_Init();
	DS1307_GetTimeDate(&hour, &minute, &second, &day, &month, &year);
	
	sei();
	
	_delay_ms(1000);
	
	display_set_display(NOKIA_3310_DISPLAY);
		display_clear();
		
	display_set_display(SH1106_DISPLAY);
		display_clear();

    while(1)
    {
		if(state == 0)
		{
			update_main_screen();
			
			display_set_display(NOKIA_3310_DISPLAY);
				display_clear();
				draw_main_screen();
				display_draw();
				
			display_set_display(SH1106_DISPLAY);
				display_clear();
				draw_main_screen();
				display_draw();
		}
		else
		{
			update_menu();
			
			display_set_display(NOKIA_3310_DISPLAY);
				display_clear();
				draw_menu();
				display_draw();
				
			display_set_display(SH1106_DISPLAY);
				display_clear();
				draw_menu();
				display_draw();
		}
		
		_delay_ms(50);
    }
}

//Przerwanie timera 0
ISR(TIMER0_OVF_vect)
{
	//Pomiar (1s)
	if(measurement_overflows >= 7812)
	{
		make_measurement = 1;
		measurement_overflows = 0;
	}
	else measurement_overflows ++;
	
	//Sprawdzanie stanu przyciska enkodera
	if(encoder_btn_overflows >= 40)
	{
		encoder_update();
		
		encoder_btn_overflows = 0;
	}
	else encoder_btn_overflows ++;
	
	TCNT0 = 0;
}