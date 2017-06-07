/**
 * @file	ei_widget.h
 *
 * @brief 	API for widgets management: creation, configuration, hierarchy, redisplay.
 *
 *  Created by François Bérard on 30.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 */

#ifndef EI_ALL_WIDGETS_H
#define EI_ALL_WIDGETS_H

#include "ei_widget.h"
#include "ei_event.h"

/*
 *\brief Definition of a callback widget (Used for widget destroy notify)
 *
 */
typedef struct ei_callback_widget_t {
    ei_widget_t widget;
    ei_callback_t		callback;
    void*			user_param;
} ei_callback_widget_t;

/*
 *\brief Definition of a frame which can be used to draw a simple frame with 3D effect or not.
 *
 */
typedef struct ei_frame_t {
    ei_callback_widget_t widget;
    ei_color_t* color;
    int* border_width;
    ei_relief_t* relief;
    char **text;
    ei_font_t* text_font;
    ei_color_t* text_color;
    ei_anchor_t* text_anchor;
    ei_surface_t* img;
    ei_rect_t** img_rect;
    ei_anchor_t* img_anchor;
} ei_frame_t;

/*
 *\brief Definition of a button which can be used to draw a button.
 *
 */
typedef struct ei_button_t {
    ei_callback_widget_t widget;
    ei_color_t* color;
    int*		border_width;
    int*		corner_radius;
    ei_relief_t*	relief;
    char **text;
    ei_font_t* text_font;
    ei_color_t* text_color;
    ei_anchor_t* text_anchor;
    ei_surface_t* img;
    ei_rect_t** img_rect;
    ei_anchor_t* img_anchor;
    ei_callback_t*		callback;
    void**			user_param;
} ei_button_t;

/*
 *\brief Definition of a toplevel which can be used to draw top level.
 *
 */
typedef struct ei_toplevel_t {
    ei_callback_widget_t widget;
    ei_color_t* color;
    int*		border_width;
    char **title;
    ei_bool_t* closable;
    ei_axis_set_t* resizable;
    ei_size_t** min_size;
    ei_widget_t* button_closable;
} ei_toplevel_t;

/**
 * @brief	Returns the widget that has the given id
 *
 * @param	where		The location on screen, expressed in the root window coordinates.
 * @param   pick_id     The Id that we are looking for
 *
 * @return  widget with the correct ID
 */
ei_widget_t* ei_find_pick_color(ei_widget_t* widget, uint32_t pick_id);


/*
 * \brief Frees all widgets from widget.
 *
 * \param   widget  the widget from which to free.
 */
void free_widgets(ei_widget_t* widget);


/**
 * \brief	Draws all the widgets
 *
 * @param   widget  root widget from which it draws the widgets
 *
 */
void draw_widgets(ei_widget_t* widget);


/**
 * \brief  The function gives the top_left from which to draw a text or an
 * image in the rectangle specified
 *
 * @param   rectangle  The rectangle where to draw
 * @param   anchor  The anchor of the rectangle
 *
 * @return  The top_left point to start drawing
 */
ei_point_t* ei_get_where(ei_rect_t rectangle, ei_anchor_t* anchor, int border_width, ei_size_t size);

/**
 * @brief	Converts pick_id to pick_color.
 *
 *
 * @param	id      The pick_id.
 *
 * @return			Returns the pick color.
 */
ei_color_t* convert_pick_id_to_pick_color(uint32_t id);

/**
 * @brief	Changes the design of the button specified
 *
 * @param	relief	The relief that we change to
 * @param   widget The current widget
 */
void ei_change_relief_button(ei_relief_t* relief, ei_widget_t* widget);

/**
 * @brief	Find the intersetion between two rectangles
 *
 * @param	rect1 first rectangle
 * @param   rect2 second rectangle
 *
 * @return			Returns the intersection between 2 rectangles
 */
ei_rect_t* ei_intersection(ei_rect_t* rect1, ei_rect_t* rect2) ;

/**
 * @brief	Tests if an event is on the banner of a toplevel
 *
 * @param	event The event to test
 * @param   widget The current widget
 *
 * @return			Returns EI_TRUE if event is on the toplevel's banner else
 * EI_FALSE
 */
ei_bool_t is_on_the_banner(ei_widget_t* widget, ei_event_t* event);

/**
 * @brief	Tests if an event is on the bottom right square (rezisable emplacement)
 *
 * @param	event The event to test
 * @param   widget The current widget
 *
 * @return			Returns EI_TRUE if event is on the square else
 * EI_FALSE
 */
ei_bool_t is_on_the_square(ei_widget_t* widget, ei_event_t* event);

/**
 * @brief	Finds the intersection between rect1 and all of the rectangles of rect2
 *
 * @param	rect1 first rectangle
 * @param   rect2 linked rectangle
 */
void ei_intersection_linked_rect(ei_rect_t* rect1, ei_linked_rect_t* rect2);

/**
 * @brief	Find the union between two rectangles
 *
 * @param	rect1 first rectangle
 * @param   rect2 second rectangle
 *
 * @return			Returns the rectangle that contains the 2 rectangles
 */
ei_rect_t* ei_union(ei_rect_t* rect1, ei_rect_t* rect2);

/**
 * @brief   Gets the previous widget among the sons of its parent.
 *
 * @param   widget  The widget that is concerned.
 *
 * @return			Returns the pick color.
 */
ei_widget_t*    ei_widget_previous (ei_widget_t* widget);

/**
 * @brief   Draws the widgets.
 *
 */
void draw();


/*
 * \brief frees all the registered classes
 *
 */
void free_class();

/*Declaration of all the global variables of the library */
ei_widgetclass_t* LIB;
ei_widget_t* ROOT;
ei_surface_t SURFACE_ROOT;
ei_surface_t SURFACE_PICK;
ei_bool_t SORTIE;
ei_widget_t* EVENT_ACTIVE;
ei_point_t* WIN_MOVE;
ei_point_t* WIN_RESIZ;
ei_default_handle_func_t DEF_FUNC;
ei_linked_rect_t* DRAW_RECT;

#endif
