/**
 *  @file	ei_placer.c
 *  @brief	Manages the positionning and sizing of widgets on the screen.
 *
 *  \author
 *  Created by François Bérard on 18.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */

#include "ei_placer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ei_widget.h"
#include "ei_all_widgets.h"
#include "ei_widget.h"
#include "ei_widget_frame.h"
#include "ei_widget_button.h"
#include "ei_widget_toplevel.h"

/**
 * \brief	Configures the geometry of a widget using the "placer" geometry manager.
 * 		If the widget was already managed by another geometry manager, then it is first
 *		removed from the previous geometry manager.
 * 		If the widget was already managed by the "placer", then this calls simply updates
 *		the placer parameters: arguments that are not NULL replace previous values.
 * 		When the arguments are passed as NULL, the placer uses default values (detailed in
 *		the argument descriptions below). If no size is provided (either absolute or
 *		relative), then the requested size of the widget is used, i.e. the minimal size
 *		required to display its content.
 *
 * @param	widget		The widget to place.
 * @param	anchor		How to anchor the widget to the position defined by the placer
 *				(defaults to ei_anc_northwest).
 * @param	x		The absolute x position of the widget (defaults to 0).
 * @param	y		The absolute y position of the widget (defaults to 0).
 * @param	width		The absolute width for the widget (defaults to the requested width
 *				of the widget if rel_width is NULL, or 0 otherwise).
 * @param	height		The absolute height for the widget (defaults to the requested height
 *				of the widget if rel_height is NULL, or 0 otherwise).
 * @param	rel_x		The relative x position of the widget: 0.0 corresponds to the left
 *				side of the master, 1.0 to the right side (defaults to 0.0).
 * @param	rel_y		The relative y position of the widget: 0.0 corresponds to the top
 *				side of the master, 1.0 to the bottom side (defaults to 0.0).
 * @param	rel_width	The relative width of the widget: 0.0 corresponds to a width of 0,
 *				1.0 to the width of the master (defaults to 0.0).
 * @param	rel_height	The relative height of the widget: 0.0 corresponds to a height of 0,
 *				1.0 to the height of the master (defaults to 0.0).
 */
#define max(a,b) ((a) > (b) ? a : b)
#define min(a,b) ((a) < (b) ? a : b)

void			ei_place			(struct ei_widget_t*	widget,
        ei_anchor_t*		anchor,
        int*			x,
        int*			y,
        int*			width,
        int*			height,
        float*			rel_x,
        float*			rel_y,
        float*			rel_width,
        float*			rel_height)
{
    widget -> placer_params = calloc(1, sizeof(ei_placer_params_t));
    if (anchor != NULL) {
        widget -> placer_params -> anchor = anchor;
        widget -> placer_params -> anchor_data = *anchor;
    }
    else{
        widget -> placer_params -> anchor_data = ei_anc_none;
    }
    if (x != NULL) {
        widget -> placer_params -> x = x;
        widget -> placer_params -> x_data = *x;
    }
    else{
        widget -> placer_params -> x_data = 0;
    }
    if (y != NULL) {
        widget -> placer_params -> y = y;
        widget -> placer_params -> y_data = *y;
    }
    else{
        widget -> placer_params -> y_data = 0;
    }
    if (height != NULL) {
        widget -> placer_params -> h = height;
        widget -> placer_params -> h_data = *height;
    }
    else{
        widget -> placer_params -> h_data = (widget -> requested_size).height;
    }
    if (width != NULL) {
        widget -> placer_params -> w = width;
        widget -> placer_params -> w_data = *width;
    }
    else{
        widget -> placer_params -> w_data = (widget -> requested_size).width;
    }
    if (rel_x != NULL) {
        widget -> placer_params -> rx = rel_x;
        widget -> placer_params -> rx_data = *rel_x;
    }
    else{
        widget -> placer_params -> rx_data = 0;
    }
    if (rel_y != NULL) {
        widget -> placer_params -> ry = rel_y;
        widget -> placer_params -> ry_data = *rel_y;
    }
    else{
        widget -> placer_params -> ry_data = 0;
    }
    if (rel_height != NULL) {
        widget -> placer_params -> rh = rel_height;
        widget -> placer_params -> rh_data = *rel_height;
    }
    else{
        widget -> placer_params -> rh_data = 0;
    }
    if (rel_width != NULL) {
        widget -> placer_params -> rw = rel_width;
        widget -> placer_params -> rw_data = *rel_width;
    }
    else{
        widget -> placer_params -> rw_data = 0;
    }
}


/**
 * \brief	Tells the placer to recompute the geometry of a widget.
 *		The widget must have been previsouly placed by a call to \ref ei_place.
 *		Geometry re-computation is necessary for example when the text label of
 *		a widget has changed, and thus the widget "natural" size has changed.
 *
 * @param	widget		The widget which geometry must be re-computed.
 */
void ei_placer_run(struct ei_widget_t* widget){
    ei_anchor_t		anchor = (widget -> placer_params) -> anchor_data;
    int			x = (widget -> placer_params) -> x_data;
    int			y = (widget -> placer_params) -> y_data;
    int			width = (widget -> placer_params) -> w_data;
    int			height = (widget -> placer_params) -> h_data;
    float			rel_x = (widget -> placer_params) -> rx_data;
    float			rel_y = (widget -> placer_params) -> ry_data;
    float			rel_width = (widget -> placer_params) -> rw_data;
    float 		rel_height = (widget -> placer_params) -> rh_data;
    ei_rect_t* rect_widget = &(widget -> screen_location);
    //===============================Managing rect size========================
    (rect_widget -> size).height = max((((widget -> parent) -> screen_location).size.height) * (rel_height), height);
    (rect_widget -> size).width = max((((widget -> parent) -> screen_location).size.width) * (rel_width), width);
    //===============================Managing anchor========================
    ei_point_t point_ancre;
    int parent_height = ((widget -> parent) -> screen_location).size.height;
    if (strcmp(widget -> parent -> wclass -> name, "toplevel") == 0) {
        ei_toplevel_t* toplevel = (ei_toplevel_t*) widget -> parent;
        if (toplevel -> button_closable != widget) {
            y += 30;
            parent_height -= 30;
        }
    }
    int parent_width = ((widget -> parent) -> screen_location).size.width;
    ei_point_t parent_origin = ((widget -> parent) -> screen_location).top_left;
    point_ancre.x = parent_origin.x + parent_width * (rel_x) + x;
    point_ancre.y = parent_origin.y + parent_height * (rel_y) + y;
    switch (anchor) {
        case ei_anc_none:
            rect_widget -> top_left = point_ancre;
            break;
        case ei_anc_northwest:
            rect_widget -> top_left = point_ancre;
            break;
        case ei_anc_west:
            rect_widget -> top_left.x = point_ancre.x;
            rect_widget -> top_left.y = point_ancre.y - 0.5 * (rect_widget -> size.height);
            break;
        case ei_anc_southwest:
            rect_widget -> top_left.x = point_ancre.x;
            rect_widget -> top_left.y = point_ancre.y - (rect_widget -> size.height);
            break;
        case ei_anc_south:
            rect_widget -> top_left.x = point_ancre.x - 0.5 * (rect_widget -> size.width);
            rect_widget -> top_left.y = point_ancre.y - (rect_widget -> size.height);
            break;
        case ei_anc_southeast:
            rect_widget -> top_left.x = point_ancre.x - (rect_widget -> size.width);
            rect_widget -> top_left.y = point_ancre.y - (rect_widget -> size.height);
            break;
        case ei_anc_east:
            rect_widget -> top_left.x = point_ancre.x - (rect_widget -> size.width);
            rect_widget -> top_left.y = point_ancre.y - 0.5*(rect_widget -> size.height);
            break;
        case ei_anc_northeast:
            rect_widget -> top_left.x = point_ancre.x - (rect_widget -> size.width);
            rect_widget -> top_left.y = point_ancre.y;
            break;
        case ei_anc_north:
            rect_widget -> top_left.x = point_ancre.x - 0.5 * (rect_widget -> size.width);
            rect_widget -> top_left.y = point_ancre.y;
            break;
        case ei_anc_center:
            rect_widget -> top_left.x = point_ancre.x - 0.5 * (rect_widget -> size.width);
            rect_widget -> top_left.y = point_ancre.y - 0.5 * (rect_widget -> size.height);
            break;
        default:
            break;
    }

}

/**
 * \brief	Tells the placer to remove a widget from the screen and forget about it.
 *		Note: the widget is not destroyed and still exists in memory.
 *
 * @param	widget		The widget to remove from screen.
 */
void ei_placer_forget(struct ei_widget_t* widget){
    free(widget -> placer_params);
}
