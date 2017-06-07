#include <stdlib.h>
#include "ei_draw.h"
#include "ei_types.h"
#include "ei_main.h"
#include "ei_event.h"
#include "ei_utils.h"
#include "hw_interface.h"
#include <stdio.h>
#include "ei_draw_widgets.h"
#include "ei_draw_extension.h"
#include "ei_draw_poly.h"


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
	color2 -> red = 255;
	color2 -> green = 0;
	color2 -> blue = 0;
	color2 -> alpha = 100;
	ei_color_t* color3;
	color3 = malloc(sizeof(ei_color_t));
	color3 -> red = 255;
	color3 -> green = 255;
	color3 -> blue = 255;
	color3 -> alpha = 0;
	char* text = "Button";
	ei_size_t size = {200, 100};
	ei_point_t top_left = {100, 100};
	ei_rect_t rectangle = {top_left, size};
	hw_init();
	main_window = hw_create_window(&main_window_size, EI_FALSE);
	ei_surface_t text_surface = hw_text_create_surface(text, ei_default_font, color2);
	ei_size_t text_size = hw_surface_get_size(text_surface);
	ei_fill(main_window, color, NULL);
	ei_point_t where = {rectangle.top_left.x + (rectangle.size.width/2) - (text_size.width/2),rectangle.top_left.y + (rectangle.size.height / 2)  - (text_size.height/2)};
	ei_draw_button(main_window, rectangle, *color2, 20, 10,ei_relief_raised, &text, ei_default_font, color3,NULL, &rectangle, where,NULL);
	event.type = ei_ev_none;
	while (event.type != ei_ev_keydown)
		hw_event_wait_next(&event);
	ei_size_t size2 = {200, 100};
	ei_point_t top_left2 = {100, 100};
	ei_rect_t rectangle2 = {top_left2, size2};
	char* text2 = "Pressed";
	ei_color_t* color5;
	color5 = malloc(sizeof(ei_color_t));
	ei_color_t* color4;
	color4 = malloc(sizeof(ei_color_t));
	color4 -> red = 255;
	color4 -> green = 0;
	color4 -> blue = 0;
	color4 -> alpha = 0;
	color5 -> red = 255;
	color5 -> green = 255;
	color5 -> blue = 255;
	color5 -> alpha = 0;
	ei_surface_t text_surface2 = hw_text_create_surface(text2, ei_default_font, color5);
	ei_size_t text_size2 = hw_surface_get_size(text_surface2);
	ei_point_t where2 = {rectangle2.top_left.x + (rectangle2.size.width/2) - (text_size2.width/2),rectangle2.top_left.y + (rectangle2.size.height / 2)  - (text_size2.height/2)};
	ei_draw_button(main_window, rectangle2, *color4, 20, 10,ei_relief_sunken, &text2, ei_default_font, color5, NULL, &rectangle2, where2, NULL);
	event.type = ei_ev_none;
	while (event.type != ei_ev_keyup)
		hw_event_wait_next(&event);
	ei_fill(main_window, color, NULL);
	ei_size_t size3 = {220, 220};
	ei_point_t top_left3 = {100, 100};
	ei_rect_t rectangle3 = {top_left3, size3};
	ei_surface_t img = hw_image_load("misc/klimt.jpg", main_window);
	ei_point_t img_point = {0,0};
	ei_size_t img_size = {200,200};
	ei_rect_t img_rect = {img_point, img_size};
	ei_point_t where3 = {rectangle2.top_left.x + 10,rectangle2.top_left.y + 10};
	ei_draw_button(main_window, rectangle3, *color4, 0, 10,ei_relief_raised, NULL, ei_default_font, color5, &img, &img_rect, where3, NULL);
	hw_surface_update_rects(main_window, NULL);

	event.type = ei_ev_none;
	while (event.type != ei_ev_keydown)
		hw_event_wait_next(&event);
	// Free hardware resources.
	// hw_text_font_free(font);
	hw_quit();

	// Terminate program with no error code.
	return 0;
}
