#include <stdlib.h>
#include "ei_draw.h"
#include "ei_types.h"
#include "ei_main.h"
#include "ei_event.h"
#include "ei_utils.h"
#include "hw_interface.h"
#include <stdio.h>
#include "ei_arc.h"


int ei_main(int argc, char** argv)
{
	ei_surface_t			main_window		= NULL;
	ei_size_t			main_window_size	= ei_size(600, 600);
	ei_event_t			event;
  ei_color_t* color;
  color = malloc(sizeof(ei_color_t));
  color -> red = 255;
  color -> green = 255;
  color -> blue = 255;
  color -> alpha = 0xAB;
	ei_color_t* color2;
	color2 = malloc(sizeof(ei_color_t));
	color2 -> red = 119;
	color2 -> green = 119;
	color2 -> blue = 119;
	color2 -> alpha = 100;
	ei_color_t* color3;
	color3 = malloc(sizeof(ei_color_t));
	color3 -> red = 255;
	color3 -> green = 255;
	color3 -> blue = 255;
	color3 -> alpha = 0;
	char* text2 = "Button";
	ei_size_t size = {200, 100};
	ei_point_t top_left = {100, 100};
	ei_rect_t rectangle = {top_left, size};
	hw_init();
	main_window = hw_create_window(&main_window_size, EI_FALSE);
	ei_fill(main_window, color, NULL);
	ei_draw_button(main_window, rectangle, 30, *color2, *color3, text2, EI_TRUE, ei_default_font, NULL);
	ei_size_t size2 = {200, 100};
	ei_point_t top_left2 = {300, 300};
	ei_rect_t rectangle2 = {top_left2, size2};
	ei_draw_button(main_window, rectangle2, 30, *color2, *color3, text2, EI_FALSE, ei_default_font, NULL);
  event.type = ei_ev_none;
	while (event.type != ei_ev_keydown)
		hw_event_wait_next(&event);
	// Free hardware resources.
	// hw_text_font_free(font);
	hw_quit();

	// Terminate program with no error code.
	return 0;
}
