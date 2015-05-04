#include "Diagnostics.h"

void init_diagnostics(void)
{
	DDRA &=~ (1<<0); 
	
	ADCSRA = (1<<ADEN)
			|(1<<ADPS0)
			|(1<<ADPS1)
			|(1<<ADPS2
			|(1<<ADATE)
			|(1<<ADSC));		
}

void update_diagostics(void)
{
	if(is_encoder_btn_pressed())
	{
		screen_mode = 0;
		
		DDRA |= (1 << 0);
		ADCSRA = 0;
	}
		
	rtc_voltage = ((float)ADC / 1024.0f) * 5;
	dtostrf(rtc_voltage, 1, 2, rtc_voltage_buffer);
}

void draw_diagnostics(void)
{	
	char rtc_voltage_display;
	sprintf(&rtc_voltage_display,"%sV", rtc_voltage_buffer);
	
	display_custom_text("Diagnostyka", 0, 0, 84, 8, 1, 0, 1);
	
	display_custom_text(&rtc_voltage_display, 0, 8, 84, 32, 1, 1, 2);
	display_custom_text("Napiêcie RTC", 0, 0, 84, 46, 1, 2, 1);
}