/*
 *  puzzle.c
 *  ig
 *
 *  Created by François Bérard on 30.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */


#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_widget.h"
#include "ei_utils.h"
#include "ei_event.h"


static const int		k_tile_size			= 128;
static char*			k_default_image_filename	= "misc/klimt.jpg";
static ei_point_t		g_toplevel_position		= {30, 10};

struct puzzle_t;

typedef struct tile_t {

	struct puzzle_t*	puzzle;
	ei_point_t		original_position;
	ei_point_t		current_position;
	ei_widget_t*		button;

} tile_t;

typedef struct puzzle_t {

	ei_size_t		n;
	tile_t*			tiles;
	tile_t**		current;

} puzzle_t;

static inline ei_point_t place_coordinates(ei_point_t pos)
{
	return ei_point(pos.x*k_tile_size, pos.y*k_tile_size);
}

static inline ei_bool_t valid(puzzle_t* puzzle, ei_point_t position)
{
	return 	(position.x >= 0) &&
		(position.x < puzzle->n.width) &&
		(position.y >= 0) &&
		(position.y < puzzle->n.height);
}

static inline int index_at(puzzle_t* puzzle, ei_point_t position)
{
	return position.y * puzzle->n.width + position.x;
}

static inline tile_t* tile_memory_at(puzzle_t* puzzle, ei_point_t position)
{
	return &(puzzle->tiles[index_at(puzzle, position)]);
}

static inline tile_t* tile_at(puzzle_t* puzzle, ei_point_t position)
{
	return puzzle->current[index_at(puzzle, position)];
}

void handle_tile_press(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	tile_t*		tile		= (tile_t*)user_param;
	puzzle_t*	puzzle		= tile->puzzle;
	ei_point_t	offsets[]	= {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};
	ei_point_t	current		= tile->current_position;
	ei_point_t	swap_pos;
	ei_point_t	swap_coordinates;
	int		i;
	
	for (i = 0; i < 4; i++) {
		swap_pos	= ei_point_add(current, offsets[i]);
		if (valid(puzzle, swap_pos) && tile_at(puzzle, swap_pos) == NULL) {
			puzzle->current[index_at(puzzle, current)]	= NULL;
			puzzle->current[index_at(puzzle, swap_pos)]	= tile;
			tile->current_position				= swap_pos;
			swap_coordinates				= place_coordinates(swap_pos);
			ei_place(tile->button, NULL, &(swap_coordinates.x), &(swap_coordinates.y),
									NULL, NULL, NULL, NULL, NULL, NULL);
		}
	}
}

void create_puzzle_window(char* image_filename)
{
	ei_widget_t*		toplevel;
	ei_surface_t		image;
	ei_size_t		image_size;
	int			x, y;
	int			border_width;
	ei_point_t		place_coords;
	ei_point_t		current_position;
	ei_size_t		n;
	ei_size_t		toplevel_size;
	ei_size_t		image_rect_size;
	ei_size_t		tile_size		= ei_size(k_tile_size, k_tile_size);
	char*			title			= "Puzzle";
	ei_color_t		toplevel_bg		= {0xff, 0xff, 0xff, 0x60};
	ei_axis_set_t		resizable		= ei_axis_none;

	ei_widget_t*		button;
	int			corner_radius		= 0;
	ei_color_t		grey			= {0x88, 0x88, 0x88, 0xff};
	ei_relief_t		relief			= ei_relief_raised;
	ei_rect_t		img_rect;
	ei_rect_t*		img_rect_ptr		= &img_rect;
	ei_callback_t		callback		= handle_tile_press;
	
	puzzle_t*		puzzle;
	tile_t*			tile;

	image		= hw_image_load(image_filename, ei_app_root_surface());
	image_size	= hw_surface_get_size(image);
	n		= ei_size(image_size.width / k_tile_size, image_size.height / k_tile_size);

	toplevel	= ei_widget_create("toplevel", ei_app_root_widget());
	toplevel_size	= ei_size(n.width*k_tile_size, n.height*k_tile_size);
	border_width	= 0;
	ei_toplevel_configure(toplevel, &toplevel_size, &toplevel_bg, &border_width, &title, NULL, &resizable, NULL);
	ei_place(toplevel, NULL, &(g_toplevel_position.x), &(g_toplevel_position.y), NULL, NULL, NULL, NULL, NULL, NULL);
	g_toplevel_position = ei_point_add(g_toplevel_position, ei_point(12, 12));

	puzzle		= (puzzle_t*)malloc(sizeof(*puzzle));
	puzzle->n	= n;
	puzzle->tiles	= (tile_t*) malloc(n.width*n.height*sizeof(tile_t));
	puzzle->current	= (tile_t**)malloc(n.width*n.height*sizeof(tile_t*));

	border_width	= 2;
	image_rect_size	= ei_size_sub(tile_size, ei_size(2*border_width, 2*border_width));

	for (y = 0; y < n.height; y++) {
		for (x = 0; x < n.width; x++) {
			current_position	= ei_point(x, y);
			if ((x == n.width - 1) && (y == n.height - 1)) {
				puzzle->current[index_at(puzzle, current_position)] = NULL;
				continue;
			}
			button			= ei_widget_create("button", toplevel);
			img_rect		= ei_rect(ei_point(x*k_tile_size+border_width, y*k_tile_size+border_width),
								image_rect_size);
			tile			= tile_memory_at(puzzle, ei_point(x, y));
			ei_button_configure(button, &tile_size, &grey, &border_width, &corner_radius, &relief, NULL, NULL, NULL,
								NULL, &image, &img_rect_ptr, NULL, &callback, (void*)&tile);
			place_coords		= place_coordinates(current_position);
			ei_place(button, NULL, &(place_coords.x), &(place_coords.y), NULL, NULL, NULL, NULL, NULL, NULL);

			tile->puzzle		= puzzle;
			tile->button		= button;
			tile->original_position	= current_position;
			tile->current_position	= current_position;

			puzzle->current[index_at(puzzle, current_position)] = tile;
		}
	}
}



// global_handler --

ei_bool_t global_handler(ei_event_t* event)
{
	uint32_t	cmd_mask = (1 << ei_mod_ctrl_left) | (1 << ei_mod_ctrl_right) |
					(1 << ei_mod_meta_left) | (1 << ei_mod_meta_right);

	if (event->type == ei_ev_keydown) {
		if (event->param.key.key_sym == SDLK_ESCAPE) {
			ei_app_quit_request();
			return EI_TRUE;
		}

		if (event->param.key.modifier_mask & cmd_mask) {
			if (event->param.key.key_sym == SDLK_n) {
				create_puzzle_window(k_default_image_filename);
				return EI_TRUE;
			}
		}
	}

	return EI_FALSE;
}



int ei_main(int argc, char* argv[])
{
	ei_size_t	root_window_size		= { 1024, 768 };
	ei_color_t	root_bgcol			= { 0x52, 0x7f, 0xb4, 0xff };
	ei_bool_t	fullscreen			= EI_FALSE;

	ei_app_create(&root_window_size, fullscreen);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	
	ei_event_set_default_handle_func(global_handler);

	if (argc > 1)
		create_puzzle_window(argv[1]);
	else
		create_puzzle_window(k_default_image_filename);

	ei_app_run();
	
	ei_app_free();

	return (EXIT_SUCCESS);
}
