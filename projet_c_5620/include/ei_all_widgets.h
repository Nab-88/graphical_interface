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
 *\brief Definition of a frame which can be used to draw a simple frame with 3D effect or not.
 *
 */
typedef struct ei_frame_t {
    ei_widget_t widget;

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
    ei_widget_t widget;

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
 *\brief Definition of a button which can be used to draw top level.
 *
 */
typedef struct ei_toplevel_t {
    ei_widget_t widget;
    ei_color_t* color;
    int*		border_width;
    char **title;
    ei_bool_t* closable;
    ei_axis_set_t* resizable;
    ei_size_t** min_size;
    ei_widget_t* button_closable;
} ei_toplevel_t;

/**
 * @brief	Returns the widget that is has the given id
 *
 * @param	where		The location on screen, expressed in the root window coordinates.
 * @param   pick_id     The Id that we are looking for
 *
 * @return
 */
ei_widget_t* ei_find_pick_color(ei_widget_t* widget, uint32_t pick_id);

/**
 * \brief	Registers the "frame" widget class in the program. This must be called only
 *		once before widgets of the class "toplevel" can be created and configured with
 *		\ref ei_toplevel_configure.
 */
void			ei_toplevel_register_class 	();

/**
 * \brief	Registers the "button" widget class in the program. This must be called only
 *		once before widgets of the class "toplevel" can be created and configured with
 *		\ref ei_toplevel_configure.
 */
void			ei_button_register_class 	();

/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a widget of a class. After allocation, the function *must*
 *		initialize the memory to 0.
 *
 * @return		A block of memory with all bytes set to 0.
 */
void*	ei_frame_allocfunc_t		();

/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a widget of a class. After allocation, the function *must*
 *		initialize the memory to 0.
 *
 * @return		A block of memory with all bytes set to 0.
 */
void*	ei_toplevel_allocfunc_t		();

/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a widget of a class. After allocation, the function *must*
 *		initialize the memory to 0.
 *
 * @return		A block of memory with all bytes set to 0.
 */
void*	ei_button_allocfunc_t		();

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
 * \brief	A function that releases the memory used by a widget before it is destroyed.
 *		The \ref ei_widget_t structure itself, passed as parameter, must *not* by freed by
 *		these functions. Can be set to NULL in \ref ei_widgetclass_t if no memory is used by
 *		a class of widget.
 *
 * @param	widget		The widget which resources are to be freed.
 */

void	ei_toplevel_releasefunc_t	(struct ei_widget_t*	widget);

/**
 * \brief	A function that releases the memory used by a widget before it is destroyed.
 *		The \ref ei_widget_t structure itself, passed as parameter, must *not* by freed by
 *		these functions. Can be set to NULL in \ref ei_widgetclass_t if no memory is used by
 *		a class of widget.
 *
 * @param	widget		The widget which resources are to be freed.
 */

void	ei_button_releasefunc_t	(struct ei_widget_t*	widget);
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
* \brief	A function that draws widgets of a class.
*
* @param	widget		A pointer to the widget instance to draw.
* @param	surface		Where to draw the widget. The actual location of the widget in the
*				surface is stored in its "screen_location" field.
* @param	clipper		If not NULL, the drawing is restricted within this rectangle
*				(expressed in the surface reference frame).
*/
void	ei_toplevel_drawfunc_t		(struct ei_widget_t*	widget,
					 ei_surface_t		surface,
					 ei_surface_t		pick_surface,
						 ei_rect_t*		clipper);

/**
* \brief	A function that draws widgets of a class.
*
* @param	widget		A pointer to the widget instance to draw.
* @param	surface		Where to draw the widget. The actual location of the widget in the
*				surface is stored in its "screen_location" field.
* @param	clipper		If not NULL, the drawing is restricted within this rectangle
*				(expressed in the surface reference frame).
*/
void	ei_button_drawfunc_t		(struct ei_widget_t*	widget,
					 ei_surface_t		surface,
					 ei_surface_t		pick_surface,
						 ei_rect_t*		clipper);

/**
* \brief	A function that sets the default values for a class.
*
* @param	widget		A pointer to the widget instance to intialize.
*/
void	ei_toplevel_setdefaultsfunc_t	(struct ei_widget_t*	widget);

/**
* \brief	A function that sets the default values for a class.
*
* @param	widget		A pointer to the widget instance to intialize.
*/
void	ei_button_setdefaultsfunc_t	(struct ei_widget_t*	widget);
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
* \brief 	A function that is called to notify the widget that its geometry has been modified
*		by its geometry manager. Can set to NULL in \ref ei_widgetclass_t.
*
* @param	widget		The widget instance to notify of a geometry change.
* @param	rect		The new rectangular screen location of the widget
*				(i.e. = widget->screen_location).
*/

void	ei_toplevel_geomnotifyfunc_t	(struct ei_widget_t*	widget,
							 ei_rect_t		rect);

/**
* \brief 	A function that is called to notify the widget that its geometry has been modified
*		by its geometry manager. Can set to NULL in \ref ei_widgetclass_t.
*
* @param	widget		The widget instance to notify of a geometry change.
* @param	rect		The new rectangular screen location of the widget
*				(i.e. = widget->screen_location).
*/

void	ei_button_geomnotifyfunc_t	(struct ei_widget_t*	widget,ei_rect_t rect);

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
 ei_bool_t ei_toplevel_handlefunc_t (struct ei_widget_t*	widget,
 						 struct ei_event_t*	event);

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
 ei_bool_t ei_button_handlefunc_t (struct ei_widget_t*	widget,
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
 * @brief	Changes the relief of a button
 *
 * @param	relief	The relief that we change to
 * @param   widget the current widget
 */
void ei_change_relief_button(ei_relief_t* relief, ei_widget_t* widget);
ei_rect_t* ei_intersection(ei_rect_t* rect1, ei_rect_t* rect2) ;
ei_bool_t is_on_the_banner(ei_widget_t* widget, ei_event_t* event);
ei_bool_t is_on_the_square(ei_widget_t* widget, ei_event_t* event);
void ei_intersection_linked_rect(ei_rect_t* rect1, ei_linked_rect_t* rect2);
ei_rect_t* ei_union(ei_rect_t* rect1, ei_rect_t* rect2);
/**
 * @brief   Gets the previous widget among the sons of its parent.
 *
 * @param   widget  The widget that is concerned.
 */
ei_widget_t*    ei_widget_previous (ei_widget_t* widget);

void draw();
/*Declaration of the library of class of widgets
 */
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
