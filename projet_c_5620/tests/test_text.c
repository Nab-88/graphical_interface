#include <stdlib.h>
#include "ei_draw.h"
#include "ei_types.h"
#include "ei_main.h"
#include "ei_event.h"
#include "ei_utils.h"
#include "hw_interface.h"

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
  color -> alpha = 0;
  ei_point_t* where = malloc(sizeof(ei_point_t));
  *where = ei_point(320,240);
	// Create the font from default_font_filename and font_default_size
  ei_font_t font = hw_text_font_create(ei_default_font_filename, ei_style_normal, ei_font_default_size);
  // Init acces to hardware.
	hw_init();
  // Create the main window.
  main_window = hw_create_window(&main_window_size, EI_FALSE);
	// Call the draw text function
  ei_draw_text(main_window, where, "lol",font, color, NULL);
	// Free the font created above
  hw_text_font_free(font);
	
  event.type = ei_ev_none;
	while (event.type != ei_ev_keydown)
		hw_event_wait_next(&event);
	// Free hardware resources.
	hw_quit();

	// Terminate program with no error code.
	return 0;
}
