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

/* 
 *\brief Definition of a frame which can be used to draw a simple frame with 3D effect or not.
 *
 */
typedef struct ei_frame_t {
    ei_widget_t widget;

    ei_size_t* requested_size;
    const ei_color_t* color;
    int* border_width;
    ei_relief_t* relief;
    char **text;
    ei_font_t* text_font;
    const ei_color_t* text_color;
    ei_anchor_t* text_anchor;
    ei_surface_t* img;
    ei_rect_t** img_rect;
    ei_anchor_t* img_anchor;
} ei_frame_t;
    
/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a widget of a class. After allocation, the function *must*
 *		initialize the memory to 0.
 *
 * @return		A block of memory with all bytes set to 0.
 */
void*	ei_frame_allocfunc_t		();

/**
 * \brief	A function that releases the memory used by a widget before it is destroyed.
 *		The \ref ei_widget_t structure itself, passed as parameter, must *not* by freed by
 *		these functions. Can be set to NULL in \ref ei_widgetclass_t if no memory is used by
 *		a class of widget.
 *
 * @param	widget		The widget which resources are to be freed.
 */

void	ei_frame_releasefunc_t	(struct ei_widget_t*	widget);

/**
 * \brief	A function that draws widgets of a class.
 *
 * @param	widget		A pointer to the widget instance to draw.
 * @param	surface		Where to draw the widget. The actual location of the widget in the
 *				surface is stored in its "screen_location" field.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference frame).
 */
void	ei_frame_drawfunc_t		(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper);

/**
 * \brief	A function that sets the default values for a class.
 *
 * @param	widget		A pointer to the widget instance to intialize.
 */
void	ei_frame_setdefaultsfunc_t	(struct ei_widget_t*	widget);

/**
 * \brief 	A function that is called to notify the widget that its geometry has been modified
 *		by its geometry manager. Can set to NULL in \ref ei_widgetclass_t.
 *
 * @param	widget		The widget instance to notify of a geometry change.
 * @param	rect		The new rectangular screen location of the widget
 *				(i.e. = widget->screen_location).
 */

void	ei_frame_geomnotifyfunc_t	(struct ei_widget_t*	widget,
							 ei_rect_t		rect);
/**
 * @brief	A function that is called in response to an event. This function 
 *		is internal to the library. It implements the generic behavior of
 *		a widget (for example a button looks sunken when clicked)
 *
 * @param	widget		The widget for which the event was generated.
 * @param	event		The event containing all its parameters (type, etc.)
 *
 * @return			A boolean telling if the event was consumed by the callback or not.
 *				If TRUE, the library does not try to call other callbacks for this
 *				event. If FALSE, the library will call the next callback registered
 *				for this event, if any.
 *				Note: The callback may execute many operations and still return
 *				FALSE, or return TRUE without having done anything.
 */
ei_bool_t ei_frame_handlefunc_t (struct ei_widget_t*	widget,
						 struct ei_event_t*	event);

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

/*Declaration of the library of class of widgets
 */
ei_widgetclass_t LIB;
ei_widget_t ROOT;
ei_surface_t SURFACE_ROOT;

#endif
