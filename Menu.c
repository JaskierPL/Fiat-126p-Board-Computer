#include "Menu.h"

char* menu_items[5] = {"Ekran G³ówny", "Diagnostyka", "Data/Czas", "Oœwietlenie", "Jasnoœæ"};
unsigned char menu_items_count = 5;

void update_menu()
{
	if(screen_mode == 1)
	{
		switch(screen_on)
		{
			case 0: 
				update_diagostics();
				break;
			case 1:
				update_timedate();
				break;
			default:
				screen_mode = 0;
				break;
		}
	}
	else
	{
		menu_selected += get_encoder_rot();
			
		if(menu_selected > menu_items_count - 1) menu_selected = menu_items_count - 1;
		else if(menu_selected < 0) menu_selected = 0;

		if(is_encoder_btn_pressed())
		{
			if(menu_selected == 0) state = 0;
			else 
			{
				screen_on = menu_selected - 1;
				screen_mode = 1;
				
				switch(screen_on)
				{
					case 0:
						init_diagnostics();
						break;
					case 1:
						init_timedate();
						break;
					default:
						screen_mode = 0;
						break;
				}
			}
		}
	}
}

void draw_menu()
{
	if(screen_mode == 1)
	{
		switch(screen_on)
		{
			case 0:
				draw_diagnostics();
				break;
			case 1:
				draw_timedate();
				break;
			default:
				screen_mode = 0;
				break;
		}
	}
	else
	{
		for(unsigned char i = 0; i < menu_items_count; i++)
		{
			if(menu_selected == i)
			{
				for(unsigned char y = 0; y < 84; y++)
				for(unsigned char z = 0; z < 12; z++)
				display_set_pixel(y, 20 + 8*i - menu_selected*8 + z - 2, 1);
				
				invert_mode = 1;
			}
			
			display_custom_text(menu_items[i], 4, 20 + 9*i - menu_selected*9 - (i < menu_selected ? 2 : 0) + (i > menu_selected ? 2 : 0), 84, 8, 0, 0, 1);
			invert_mode = 0;
		}
	}
}