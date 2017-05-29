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
  ei_surface_t			main_window2		= NULL;
	ei_size_t			main_window_size	= ei_size(640, 480);
	ei_event_t			event;
  ei_color_t* color;
  color = malloc(sizeof(ei_color_t));
  color -> red = 255;
  color -> green = 0;
  color -> blue = 0;
  color -> alpha = 120;

  ei_color_t* color2;
  color2 = malloc(sizeof(ei_color_t));
  color2 -> red = 0;
  color2 -> green = 255;
  color2 -> blue = 0;
  color2 -> alpha = 120;

	ei_point_t point;
	point.x = 50;
	point.y = 50;
	ei_rect_t* ls_rect;
	ls_rect = malloc(sizeof(ei_rect_t));
	ls_rect -> size = ei_size(50, 50);
	ls_rect -> top_left = point;

	ei_point_t point2;
	point2.x = 250;
	point2.y = 250;
	ei_rect_t* ls_rect2;
	ls_rect2 = malloc(sizeof(ei_rect_t));
	ls_rect2 -> size = ei_size(50, 50);
	ls_rect2 -> top_left = point2;
  // Init acces to hardware.
	hw_init();

  // Create the main window.
  main_window = hw_create_window(&main_window_size, EI_FALSE);
  ei_fill(main_window, color, ls_rect);
	event.type = ei_ev_none;
	while (event.type != ei_ev_keydown)
		hw_event_wait_next(&event);


  ei_copy_surface(main_window, ls_rect2, main_window, ls_rect, EI_FALSE);

	event.type = ei_ev_none;
	while (event.type != ei_ev_keydown)
		hw_event_wait_next(&event);

	// Free hardware resources.
	hw_quit();

	// Terminate program with no error code.
	return 0;
}
