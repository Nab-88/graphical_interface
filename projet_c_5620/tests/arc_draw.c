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

int ei_main(int argc, char** argv)
{
	ei_surface_t			main_window		= NULL;
	ei_size_t			main_window_size	= ei_size(600, 600);
	ei_event_t			event;
	uint32_t			white			= 0xffffffff;
	uint32_t*			pixel_ptr;
	int				i;

	// Init acces to hardware.
	hw_init();

	// Create the main window.
	main_window = hw_create_window(&main_window_size, EI_FALSE);

	// Lock the surface for drawing, fill in white, unlock, update screen.
	hw_surface_lock(main_window);

	pixel_ptr = (uint32_t*)hw_surface_get_buffer(main_window);
	for (i = 0; i < (main_window_size.width * main_window_size.height); i++)
		*pixel_ptr++ = white;

	hw_surface_unlock(main_window);
	hw_surface_update_rects(main_window, NULL);
	ei_point_t centre = {300, 300};
	ei_color_t color = {255, 0, 0, 0};
	ei_linked_point_t *pts = ei_arc(centre, 20, 180, 90);
	ei_draw_polyline(main_window, pts, color, NULL);

	// Wait for a key press.
	event.type = ei_ev_none;
	while (event.type != ei_ev_keydown)
		hw_event_wait_next(&event);

	// Free hardware resources.
	hw_quit();

	// Terminate program with no error code.
	return 0;
}
