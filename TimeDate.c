#include "TimeDate.h"

void init_timedate(void)
{
	DS1307_GetTimeDate(&n_hour, &n_minute, &n_second, &n_day, &n_month, &n_year);
	
	td_change_mode = 0;
	td_selected_item = 0;
}

void update_timedate(void)
{
	if(make_measurement == 1)
	{	
		DS1307_GetTimeDate(NULL, NULL, &n_second, NULL, NULL, NULL);
		make_measurement = 0;
	} 
	
	if(td_change_mode == 0)
	{
		td_selected_item -= get_encoder_rot();

		if(td_selected_item > 6) td_selected_item = 0;
		else if(td_selected_item < 0) td_selected_item = 6;
		
		if(is_encoder_btn_pressed())
		{
			if(td_selected_item == 5)
			{
				screen_mode = 0;
				DS1307_SetTimeDate(n_year, n_month, n_day, n_hour, n_minute, 0);
			}
			else if(td_selected_item == 6) screen_mode = 0;
			else td_change_mode = 1;
		}
	}
	else
	{
		signed char rot = get_encoder_rot();
		
		if(is_encoder_btn_pressed())
		{
			td_change_mode = 0;
		}
		
		else if(td_selected_item == 0)
		{
			if((signed char)n_hour - rot < 0) n_hour = 23;
			else if(n_hour - rot > 23) n_hour = 0;
			else n_hour -= rot;
		}
		else if(td_selected_item == 1)
		{
			if((signed char)n_minute - rot < 0) n_minute = 59;
			else if(n_minute - rot > 59) n_minute = 0;
			else n_minute -= rot;
		}
		
		else if(td_selected_item == 2)
		{
			if((signed char)n_day - rot < 1) n_day = 31;
			else if(n_day - rot > 31) n_day = 1;
			else n_day -= rot;
		}
		else if(td_selected_item == 3)
		{
			if((signed char)n_month - rot < 1) n_month = 12;
			else if(n_month - rot > 12) n_month = 1;
			else n_month -= rot;
		}
		else if(td_selected_item == 4)
		{
			if((signed char)n_year - rot < 0) n_year = 99;
			else if(n_year - rot > 99) n_year = 0;
			else n_year -= rot;
		}
	}
}

void draw_timedate(void)
{
	display_custom_text("Data/Czas", 0, 0, 84, 8, 1, 0, 1);
	
	char dt_string[4];
		
	if(td_change_mode && n_second % 2)
		display_custom_text("+-", 0, 0, 84, 8, 2, 0, 1);
		
	//Godzina i minuty
	if(td_selected_item == 0) invert_mode = 1;
	sprintf(dt_string, "%02u", n_hour);
	display_custom_text(dt_string, 27, 14, 84, 16, 0, 0, 1);
	invert_mode = 0;
		
	display_custom_text(":", 39, 14, 84, 16, 0, 0, 1);
	
	if(td_selected_item == 1) invert_mode = 1;
	sprintf(dt_string, "%02u", n_minute);
	display_custom_text(dt_string, 45, 14, 84, 16, 0, 0, 1);
	invert_mode = 0;
		
	//Dzieñ, miesi¹c, rok
	if(td_selected_item == 2) invert_mode = 1;
	sprintf(dt_string, "%02u", n_day);
	display_custom_text(dt_string, 12, 24, 84, 16, 0, 0, 1);
	invert_mode = 0;
	
	display_custom_text("-", 24, 24, 84, 16, 0, 0, 1);
	
	if(td_selected_item == 3) invert_mode = 1;
	sprintf(dt_string, "%02u", n_month);
	display_custom_text(dt_string, 30, 24, 84, 16, 0, 0, 1);
	invert_mode = 0;

	display_custom_text("-", 42, 24, 84, 16, 0, 0, 1);

	if(td_selected_item == 4) invert_mode = 1;
	sprintf(dt_string, "20%02u", n_year);
		display_custom_text(dt_string, 48, 24, 84, 16, 0, 0, 1);
	invert_mode = 0;
	
	//Zapis, wyœcie
	if(td_selected_item == 5) invert_mode = 1;
	display_custom_text("Zapisz", 0, 40, 84, 16, 0, 0, 1);
	invert_mode = 0;
	
	if(td_selected_item == 6) invert_mode = 1;
	display_custom_text("Anuluj", 0, 40, 84, 16, 2, 0, 1);
	invert_mode = 0;
}