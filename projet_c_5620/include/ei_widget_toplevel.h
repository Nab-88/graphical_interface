/**
 *  @file	ei_widget_toplevel.h
 *  @brief	Functions related to toplevels
 *
 */
#ifndef ei_widget_toplevel_H
#define EI_WIDGET_TOPLEVEL_H

#include "ei_draw.h"
#include "ei_widgetclass.h"
#include "ei_placer.h"
#include "ei_types.h"
#include "ei_widget.h"

/**
 * @brief	Configures the attributes of widgets of the class "toplevel".
 *
 * @param	widget		The widget to configure.
 * @param	requested_size	The content size requested for this widget, this does not include
 *				the decorations	(border, title bar). The geometry manager may
 *				override this size due to other constraints.
 *				Defaults to (320x240).
 * @param	color		The color of the background of the content of the widget. Defaults
 *				to \ref ei_default_background_color.
 * @param	border_width	The width in pixel of the border of the widget. Defaults to 4.
 * @param	title		The string title diplayed in the title bar. Defaults to "Toplevel".
 *				Uses the font \ref ei_default_font.
 * @param	closable	If true, the toplevel is closable by the user, the toplevel must
 *				show a close button in its title bar. Defaults to \ref EI_TRUE.
 * @param	resizable	Defines if the widget can be resized horizontally and/or vertically
 *				by the user. Defaults to \ref ei_axis_both.
 * @param	min_size	For resizable widgets, defines the minimum size. The default minimum
 *				size of a toplevel is (160, 120). If *min_size is NULL, this requires
 *				the toplevel to be configured to its default size.
 */
void			ei_toplevel_configure		(ei_widget_t*		widget,
        ei_size_t*		requested_size,
        ei_color_t*		color,
        int*			border_width,
        char**			title,
        ei_bool_t*		closable,
        ei_axis_set_t*		resizable,
        ei_size_t**		min_size);


/**
 * \brief	Registers the "toplevel" widget class in the program. This must be called only
 *		once before widgets of the class "toplevel" can be created and configured with
 *		\ref ei_toplevel_configure.
 */
void			ei_toplevel_register_class 	();

/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a widget of a class. After allocation, the function *must*
 *		initialize the memory to 0.
 *
 */
void*	ei_toplevel_allocfunc_t		();

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
 * \brief	A function that draws widgets of a toplevel.
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
 * \brief	A function that sets the default values for a class.
 *
 * @param	widget		A pointer to the widget instance to intialize.
 */
void	ei_toplevel_setdefaultsfunc_t	(struct ei_widget_t*	widget);
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

#endif
