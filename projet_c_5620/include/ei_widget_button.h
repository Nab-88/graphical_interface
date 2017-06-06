#ifndef EI_WIDGET_BUTTON_H
#define EI_WIDGET_BUTTON_H

#include "ei_draw.h"
#include "ei_widgetclass.h"
#include "ei_placer.h"
#include "ei_types.h"
#include "ei_widget.h"

/*
 * button_press --
 *
 *	Callback called when a user clicks on the button.
 */
void button_press2(ei_widget_t* widget, ei_event_t* event, void* user_param);


/**
 * @brief	Configures the attributes of widgets of the class "button".
 *
 * @param	widget, requested_size, color, border_width, relief,
 *		text, text_font, text_color, text_anchor,
 *		img, img_rect, img_anchor
 *				See the parameter definition of \ref ei_frame_configure. The only
 *				difference is that relief defaults to \ref ei_relief_raised
 *				and border_width defaults to \ref k_default_button_border_width.
 * @param	corner_radius	The radius (in pixels) of the rounded corners of the button.
 *				0 means straight corners. Defaults to \ref k_default_button_corner_radius.
 * @param	callback	The callback function to call when the user clicks on the button.
 *				Defaults to NULL (no callback).
 * @param	user_param	A programmer supplied parameter that will be passed to the callback
 *				when called. Defaults to NULL.
 */
void			ei_button_configure		(ei_widget_t*		widget,
        ei_size_t*		requested_size,
        const ei_color_t*	color,
        int*			border_width,
        int*			corner_radius,
        ei_relief_t*		relief,
        char**			text,
        ei_font_t*		text_font,
        ei_color_t*		text_color,
        ei_anchor_t*		text_anchor,
        ei_surface_t*		img,
        ei_rect_t**		img_rect,
        ei_anchor_t*		img_anchor,
        ei_callback_t*		callback,
        void**			user_param);

        /**
         * \brief	Registers the "button" widget class in the program. This must be called only
         *		once before widgets of the class "button" can be created and configured with
         *		\ref ei_button_configure.
         */
void			ei_button_register_class 	();

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
void	ei_button_drawfunc_t		(struct ei_widget_t*	widget,
        ei_surface_t		surface,
        ei_surface_t		pick_surface,
        ei_rect_t*		clipper);

/**
 * \brief	A function that sets the default values for a class.
 *
 * @param	widget		A pointer to the widget instance to intialize.
 */
void	ei_button_setdefaultsfunc_t	(struct ei_widget_t*	widget);


/**
 * \brief 	A function that is called to notify the widget that its geometry has been modified
 *		by its geometry manager. Can set to NULL in \ref ei_widgetclass_t.
 *
 * @param	widget		The widget instance to notify of a geometry change.
 * @param	rect		The new rectangular screen location of the widget
 *				(i.e. = widget->screen_location).
 */

void	ei_button_geomnotifyfunc_t	(struct ei_widget_t*	widget,
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
ei_bool_t ei_button_handlefunc_t (struct ei_widget_t*	widget,
        struct ei_event_t*	event);

/**
 * @brief	Changes the relief of a button
 *
 * @param	relief	The relief that we change to
 * @param   widget the current widget
 */
void ei_change_relief_button(ei_relief_t* relief, ei_widget_t* widget);

#endif
