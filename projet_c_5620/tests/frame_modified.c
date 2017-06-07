#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_utils.h"

/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size		= {600, 600};
	static char*			image_filename	= "misc/klimt.jpg";
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_widget_t*	frame;
	ei_size_t	frame_size		= {410,210};
	ei_color_t	frame_color		= {0x88, 0x88, 0x88, 100};
	ei_relief_t	frame_relief		= ei_relief_raised;
	int		frame_border_width	= 6;
	char ** text = calloc(2, sizeof(char));
	*text = "Toplevel";

	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	ei_widget_t* toplevel	= ei_widget_create("toplevel", ei_app_root_widget());
	ei_size_t toplevel_size	= ei_size(300, 300);
	int border_width	= 0;
	int ele1 = 50;
	ei_toplevel_configure(toplevel, &toplevel_size, &root_bgcol, &border_width, text, NULL, NULL, NULL);
	ei_place(toplevel, NULL, &(ele1), &(ele1), NULL, NULL, NULL, NULL, NULL, NULL);
	ei_surface_t image		= hw_image_load(image_filename, ei_app_root_surface());
	ei_size_t image_size	= hw_surface_get_size(image);
	image_size.width = 100;
	image_size.height = 300;
	ei_rect_t *img_rect		= malloc(sizeof(ei_rect_t));
	img_rect-> top_left = ei_point(0, 0);
	img_rect -> size = image_size;
	/* Create, configure and place the frame on screen. */
	frame = ei_widget_create("frame", toplevel);
	ei_frame_configure	(frame, &frame_size, &frame_color,
				 &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL,
				 &image, &img_rect, NULL);
	ei_place(frame, NULL, &ele1, &ele1, NULL, NULL, NULL, NULL, NULL, NULL );

	/* Run the application's main loop. */
	ei_app_run();
	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
