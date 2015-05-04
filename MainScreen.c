#include "MainScreen.h"

void update_main_screen()
{
	if(is_encoder_btn_pressed())
		state = 1;
	
	//Mierzenie temperatury i aktualizacja czasu
	if(make_measurement == 1)
	{
		therm_measure_begin();
		make_measurement = 0;
		
		DS1307_GetTimeDate(&hour, &minute, &second, &day, &month, &year);
	}
	
	therm_measure_fetch_results();
}

void draw_main_screen()
{
	//Wyœwietlanie temperatury
	if(therm_presence == 1)
	{
		char temp_string[10];
		sprintf(temp_string, "%s°C", therm_string_buffer);
		
		display_custom_text(temp_string, 0, 0, 84, 8, 2, 0, 1);
	}
	else
		display_custom_text("--.-°C", 0, 0, 84, 8, 2, 0, 1);
		
	//Wyœwietlanie czasu i daty
	char* day_string = "";
	char time_string[8];
	char date_string[16];
	
	sprintf(time_string, "%01u:%02u", hour, minute);
	display_custom_text(time_string, 0, 16, 84, 16, 1, 0, 2);
	
	int DayOfWeek = DS1307_GetDayOfWeek(year, month, day);
	
	switch(DayOfWeek)
	{
		case 0: 
			day_string = "ni.";
			break;
		case 1:
			day_string = "pn.";
			break;
		case 2:
			day_string = "wt.";
			break;
		case 3:
			day_string = "œr.";
			break;
		case 4:
			day_string = "cz.";
			break;
		case 5:
			day_string = "pt.";
			break;
		case 6:
			day_string = "sb.";
			break;
		default:
			day_string = "";
			break;
	}
	
	sprintf(date_string, "%s %02u-%02u-20%02u", day_string, day, month, year);
	display_custom_text(date_string, 0, 38, 84, 16, 1, 0, 1);
}