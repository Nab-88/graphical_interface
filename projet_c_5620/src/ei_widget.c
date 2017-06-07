/**
 * @file	ei_widget.c
 *
 * @brief 	API for widgets management: creation, configuration, hierarchy, redisplay.
 *
 *  Created by François Bérard on 30.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 */

#include "ei_all_widgets.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ei_draw_widgets.h"
#include "ei_draw_extension.h"
#include "ei_application.h"
#include "ei_event.h"

/**
 * @brief	Creates a new instance of a widget of some particular class, as a descendant of
 *		an existing widget.
 *
 *		The widget is not displayed on screen until it is managed by a geometry manager.
 *		The widget should be released by calling \ref ei_widget_destroy when no more needed.
 *
 * @param	class_name	The name of the class of the widget that is to be created.
 * @param	parent 		A pointer to the parent widget. Can not be NULL.
 *
 * @return			The newly created widget, or NULL if there was an error.
 */
uint32_t COLOR_ID = 1;
#define max(a,b) ((a) > (b) ? a : b)
#define min(a,b) ((a) < (b) ? a : b)





ei_widget_t*		ei_widget_create		(ei_widgetclass_name_t	class_name,
        ei_widget_t*		parent){
    ei_widgetclass_t *class = ei_widgetclass_from_name(class_name);
    ei_widget_t *widget = (*(class -> allocfunc))();
    widget -> wclass = class;
    widget -> parent = parent;
    if ( (parent -> children_head) == NULL){
        parent -> children_head = widget;
        parent -> children_tail = widget;
    } else {
        ei_widget_t* child = parent -> children_tail;
        child -> next_sibling = widget;
        parent -> children_tail = widget;
    }
    (widget -> wclass -> setdefaultsfunc)(widget);
    widget -> pick_id = COLOR_ID;
    widget -> pick_color = convert_pick_id_to_pick_color(COLOR_ID);
    COLOR_ID ++;
    return widget;

}

/**
 * @brief	Converts pick_id to pick_color.
 *
 *
 * @param	id      The pick_id.
 *
 * @return			Returns the pick color.
 */
ei_color_t* convert_pick_id_to_pick_color(uint32_t id){
    ei_color_t* pick = calloc(1, sizeof(ei_color_t));
    pick -> red = (char) (id << 24);
    pick -> green = (char) (id << 16);
    pick -> blue = (char) (id << 8);
    pick -> alpha = (char) id;
    return pick;
}


/**
 * @brief	Returns the structure describing a class, from its name.
 *
 * @param	name		The name of the class of widget.
 *
 * @return			The structure describing the class.
 */
ei_widgetclass_t*	ei_widgetclass_from_name	(ei_widgetclass_name_t name){
    ei_widgetclass_t *current = LIB;
    while (strncmp(current -> name, name, strlen(name)) != 0) {
        current = current -> next;
    }
    return current;

}
/**
 * @brief	Destroys a widget. Removes it from screen if it is managed by a geometry manager.
 *		Destroys all its descendants.
 *
 * @param	widget		The widget that is to be destroyed.
 */
void			ei_widget_destroy		(ei_widget_t*		widget){
    ei_callback_widget_t* widget_destroy = (ei_callback_widget_t*) widget;
    if (widget_destroy -> callback != NULL) {
        (widget_destroy -> callback)(widget, NULL, widget_destroy -> user_param);
    }
    free_widgets(widget -> children_tail);
    ei_widget_t* parent = (widget -> parent);
    ei_widget_t* previous = ei_widget_previous(widget);
    if (previous == NULL){
        parent -> children_head = widget -> next_sibling;
    } else if ( widget == parent -> children_tail) {
        parent -> children_tail = previous;
        previous -> next_sibling = NULL;
    } else {
        previous -> next_sibling = widget -> next_sibling;
    }
    (widget -> wclass -> releasefunc)(widget);
    ei_event_set_active_widget(NULL);
}

ei_widget_t*    ei_widget_previous (ei_widget_t* widget){
    ei_widget_t* prev = (widget -> parent) -> children_head;
    ei_widget_t* parent = (widget -> parent);
    if (widget == parent -> children_head){
        return NULL;
    }
    while (((prev -> next_sibling) != widget) && (prev -> next_sibling)
    != (parent -> children_tail) && (prev -> next_sibling) != NULL){
        prev = prev -> next_sibling;
    }
    if (prev -> next_sibling != parent -> children_tail){
        return prev;
    } else {
        if (widget == parent -> children_tail) {
            return prev;
        } else {
            return NULL;
        }
    }
}


/**
 * @brief	Returns the widget that is at a given location on screen.
 *
 * @param	where		The location on screen, expressed in the root window coordinates.
 *
 * @return			The top-most widget at this location, or NULL if there is no widget
 *				at this location (except for the root widget).
 */
ei_widget_t*		ei_widget_pick			(ei_point_t*		where){
    // Recuperer l'ID du widget qu'on cherche sur la pick_surface
    ei_surface_t pick_surface = SURFACE_PICK;
    uint32_t *pixel_ptr = (uint32_t*)hw_surface_get_buffer(pick_surface);
    ei_size_t surface_size = hw_surface_get_size(pick_surface);
    pixel_ptr += where -> x + where -> y * surface_size.width;
    int ir, ig, ib, ia;
    hw_surface_get_channel_indices(pick_surface, &ir, &ig, &ib, &ia);
    ei_color_t* color = calloc(1, sizeof(ei_color_t));
    color -> red = (unsigned char)((*pixel_ptr <<((3-ir)*8)) >> 24);
    color -> green = (unsigned char)((*pixel_ptr <<((3-ig)*8)) >> 24);
    color -> blue = (unsigned char)((*pixel_ptr <<((3-ib)*8)) >> 24);
    color -> alpha = (unsigned char)((*pixel_ptr <<((3-ia)*8)) >> 24);
    uint32_t pick_id = color -> red << 24;
    pick_id += color -> green << 16;
    pick_id += color -> blue << 8;
    pick_id += color -> alpha;
    // Parcours de tous les widgets
    ei_widget_t* current = ei_app_root_widget();
    return ei_find_pick_color(current, pick_id);

}

/**
 * @brief	Returns the widget that is has the given id
 *
 * @param	where		The location on screen, expressed in the root window coordinates.
 * @param   pick_id     The Id that we are looking for
 *
 * @return
 */
ei_widget_t* ei_find_pick_color(ei_widget_t* widget, uint32_t pick_id) {
    ei_widget_t* answer = NULL;
    while (widget != NULL){
        if (widget -> pick_id == pick_id && widget != ROOT) {
            return widget;
        }
        answer = ei_find_pick_color(widget -> children_head, pick_id);
        if (answer != NULL) {
            break;
        }
        widget = widget -> next_sibling;
    }
    return answer;
}



/**
 * \brief  The function gives the top_left from which to draw a text or an
 * image in the rectangle specified
 *
 * @param   rectangle  The rectangle where to draw
 * @param   anchor  The anchor of the rectangle
 *
 * @return  The top_left point to start drawing
 */
ei_point_t* ei_get_where(ei_rect_t rectangle, ei_anchor_t* anchor,
    int border_width, ei_size_t size) {
    ei_point_t* where = calloc(1, sizeof(ei_point_t));
    ei_point_t point_ancre = {rectangle.top_left.x + rectangle.size.width/2,
        rectangle.top_left.y + rectangle.size.height/2};
    switch (*anchor) {
        case ei_anc_none:
        case ei_anc_center:
            where -> x = point_ancre.x - 0.5 * size.width;
            where -> y = point_ancre.y - 0.5 * size.height;
            break;
        case ei_anc_northwest:
            where -> x = point_ancre.x - ((rectangle.size.width)/2) + border_width;
            where -> y = point_ancre.y - ((rectangle.size.height)/2) + border_width;
            break;
        case ei_anc_west:
            where -> x = point_ancre.x - ((rectangle.size.width)/2) + border_width;
            where -> y = point_ancre.y - 0.5 * size.height;
            break;
        case ei_anc_southwest:
            where -> x = point_ancre.x - (rectangle.size.width/2) + border_width;
            where -> y = point_ancre.y + (rectangle.size.height/2) -
            size.height - border_width;
            break;
        case ei_anc_south:
            where -> x = point_ancre.x - 0.5 * size.width;
            where -> y = point_ancre.y + (rectangle.size.height/2) -
            size.height - border_width;
            break;
        case ei_anc_southeast:
            where -> x = point_ancre.x + (rectangle.size.width/2) -
            size.width - border_width;
            where -> y = point_ancre.y + (rectangle.size.height/2) -
             size.height - border_width;
            break;
        case ei_anc_east:
            where -> x = point_ancre.x + (rectangle.size.width/2) -
            size.width - border_width;
            where -> y = point_ancre.y - 0.5 * size.height;
            break;
        case ei_anc_northeast:
            where -> x = point_ancre.x + (rectangle.size.width/2) -
            size.width - 2*border_width;
            where -> y = point_ancre.y - ((rectangle.size.height)/2) + border_width;
            break;
        case ei_anc_north:
            where -> x = point_ancre.x - 0.5 * (size.width);
            where -> y = point_ancre.y - ((rectangle.size.height)/2) + border_width;
            break;
        default:
            break;
    }
    return where;
}

ei_rect_t* ei_union(ei_rect_t* rect1, ei_rect_t* rect2) {
    ei_point_t point1 = rect1 -> top_left;
    ei_point_t point2 = rect2 -> top_left;
    ei_size_t size1 = rect1 -> size;
    ei_size_t size2 = rect2 -> size;
    ei_rect_t* intersection = malloc(sizeof(ei_rect_t));
    intersection -> top_left.x = min(point1.x, point2.x);
    intersection -> top_left.y = min(point1.y, point2.y);
    intersection -> size.width = max(size1.width, size2.width) +
    abs(point1.x - point2.x);
    intersection -> size.height = max(size1.height, size2.height) +
    abs(point1.y - point2.y);
    return intersection;
}


ei_bool_t is_on_the_banner(ei_widget_t* widget, ei_event_t* event) {
    ei_toplevel_t* toplevel = (ei_toplevel_t*) widget;
    ei_point_t where = event -> param.mouse.where;
    int x_min, x_max, y_min, y_max;
    x_min = widget -> screen_location.top_left.x;
    y_min = widget -> screen_location.top_left.y;
    x_max = widget -> screen_location.top_left.x + widget -> screen_location.size.width;
    y_max = y_min + 30 + *(toplevel -> border_width);
    if (x_min <= where.x && x_max >= where.x && y_min <= where.y
        && y_max >= where.y) {
        return EI_TRUE;
    }
    return EI_FALSE;
}

ei_bool_t is_on_the_square(ei_widget_t* widget, ei_event_t* event) {
    ei_toplevel_t* toplevel = (ei_toplevel_t*) widget;
    ei_point_t where = event -> param.mouse.where;
    int x_min, x_max, y_min, y_max;
    x_min = widget -> screen_location.top_left.x + widget -> screen_location.size.width
     - 10 - *(toplevel -> border_width);
    y_min = widget -> screen_location.top_left.y + widget -> screen_location.size.height
     - 10 - *(toplevel -> border_width);
    x_max = x_min + 10 + *(toplevel -> border_width);
    y_max = y_min + 10 + *(toplevel -> border_width);
    if (x_min <= where.x && x_max >= where.x && y_min <= where.y && y_max >= where.y) {
        return EI_TRUE;
    }
    return EI_FALSE;
}




/**
 * @brief	Registers a class to the program so that widgets of this class can be created.
 *		This must be done only once per widged class in the application.
 *
 * @param	widgetclass	The structure describing the class.
 */
void			ei_widgetclass_register		(ei_widgetclass_t* widgetclass){
    ei_widgetclass_t* current = LIB;
    while (current -> next != NULL){
        current = current -> next;
    }
    current -> next = widgetclass;
}
