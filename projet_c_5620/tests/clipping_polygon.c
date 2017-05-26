#include <stdlib.h>
#include <stdio.h>
#include "ei_types.h"
#include "ei_main.h"
#include "ei_event.h"
#include "ei_utils.h"
#include "hw_interface.h"

void test_line(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 255, 0, 255, 255 };
	ei_linked_point_t	pts[2];

	pts[0].point.x = 200; pts[0].point.y = 200; pts[0].next = &pts[1];
	pts[1].point.x = 600; pts[1].point.y = 400; pts[1].next = NULL;
	
	ei_draw_polyline(surface, pts, color, clipper);
}
int ei_main(int argc, char** argv)
{
    ei_surface_t			main_window		= NULL;
    ei_size_t			main_window_size	= ei_size(800, 600);
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
    ei_color_t		color		= { 0, 255, 0, 255 };
    ei_linked_point_t	pts[9];
    int		 xdiff, ydiff;

    /* Initialisation */
    pts[0].point.x = 400;
    pts[0].point.y = 90;

    /* Draw the octogone */
    for(i = 1; i <= 8; i++) {
        /*	Add or remove 70/140 pixels for next point
            The first term of this formula gives the sign + or - of the operation
            The second term is 2 or 1, according to which coordinate grows faster
            The third term is simply the amount of pixels to skip */
        xdiff = pow(-1, (i + 1) / 4) * pow(2, (i / 2) % 2 == 0) * 70;
        ydiff = pow(-1, (i - 1) / 4) * pow(2, (i / 2) % 2) * 70;

        pts[i].point.x = pts[i-1].point.x + xdiff;
        pts[i].point.y = pts[i-1].point.y + ydiff;
        pts[i-1].next = &(pts[i]);
    }

    /* End the linked list */
    pts[i-1].next = NULL;
    ei_rect_t clipper;
    clipper.top_left.x = 350;
    clipper.top_left.y = 200;
    ei_size_t taille;
    taille.width = 500;
    taille.height = 200;
    clipper.size = taille;
    ei_draw_polygon(main_window, pts, color, &clipper);
    // Wait for a key press.
    event.type = ei_ev_none;
    while (event.type != ei_ev_keydown)
        hw_event_wait_next(&event);

    ei_color_t		color2		= { 255, 0, 0, 255 };
    ei_linked_point_t	pts2[5];

    /* Initialisation */
    pts2[0].point.x = 300;
    pts2[0].point.y = 400;

    /* Draw the square */
    for(i = 1; i <= 4; i++) {
        /*	Add or remove 200 pixels or 0 for next point
            The first term of this formula gives the sign + or - of the operation
            The second term is 0 or 1, according to which coordinate grows
            The third term is simply the side of the square */
        xdiff = pow(-1, i / 2) * (i % 2) * 200;
        ydiff = pow(-1, i / 2) * (i % 2 == 0) * 200;

        pts2[i].point.x = pts2[i-1].point.x + xdiff;
        pts2[i].point.y = pts2[i-1].point.y + ydiff;
        pts2[i-1].next = &(pts2[i]);
    }

    /* End the linked list */
    pts2[i-1].next = NULL;

    ei_draw_polygon(main_window, pts2, color2, &clipper);

    event.type = ei_ev_none;
    while (event.type != ei_ev_keydown)
        hw_event_wait_next(&event);
    clipper.size.width = 50,
    test_line(main_window, &clipper);
    event.type = ei_ev_none;
    while (event.type != ei_ev_keydown)
        hw_event_wait_next(&event);
    // Free hardware resources.
    hw_quit();

    // Terminate program with no error code.
    return 0;
}
