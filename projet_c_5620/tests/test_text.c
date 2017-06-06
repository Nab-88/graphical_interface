#include <stdlib.h>
#include "ei_draw.h"
#include "ei_draw_widgets.h"
#include "ei_draw_extension.h"
#include "ei_draw_poly.h"
#include "ei_types.h"
#include "ei_main.h"
#include "ei_event.h"
#include "ei_utils.h"
#include "hw_interface.h"
#include <stdio.h>


int ei_main(int argc, char** argv)
{
	ei_surface_t			main_window		= NULL;
	ei_size_t			main_window_size	= ei_size(640, 480);
	ei_event_t			event;
  ei_color_t* color;
  color = malloc(sizeof(ei_color_t));
  color -> red = 255;
  color -> green = 0;
  color -> blue = 0;
  color -> alpha = 0xAB;
	ei_color_t* color2;
	color2 = malloc(sizeof(ei_color_t));
	color2 -> red = 0;
	color2 -> green = 0;
	color2 -> blue = 255;
	color2 -> alpha = 100;
	ei_color_t* color3;
	color3 = malloc(sizeof(ei_color_t));
	color3 -> red = 0;
	color3 -> green = 255;
	color3 -> blue = 0;
	color3 -> alpha = 100;
  ei_point_t* where = malloc(sizeof(ei_point_t));
  *where = ei_point(320,240);
	char* text2 = "COUCOU";
	ei_point_t point;
	point.x = 50;
	point.y = 0;
	ei_rect_t* ls_rect;
	ls_rect = malloc(sizeof(ei_rect_t));
	ls_rect -> size = ei_size(320,480);
	ls_rect -> top_left = point;
	hw_init();
	main_window = hw_create_window(&main_window_size, EI_FALSE);
	// main_window = hw_surface_create(main_window, &main_window_size, EI_TRUE);
	// uint32_t h = ei_map_rgba(main_window, color);
	// printf("%x \n", h);
	// Create the font from default_font_filename and font_default_size
  // ei_font_t font = hw_text_font_create(ei_default_font_filename, ei_style_normal, ei_font_default_size);
  // Init acces to hardware.

  // Create the main window.


	ei_fill(main_window, color, NULL);
	ei_fill(main_window, color3,ls_rect);
  ei_draw_text(main_window, where, text2, ei_default_font, color2, ls_rect);

	// Free the font created above


  event.type = ei_ev_none;
	while (event.type != ei_ev_keydown)
		hw_event_wait_next(&event);
	// Free hardware resources.
	// hw_text_font_free(font);
	hw_quit();

	// Terminate program with no error code.
	return 0;
}
