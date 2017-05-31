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
#include <string.h>
#include "ei_application.h"
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
    return widget;

}


/**
 * @brief	Returns the structure describing a class, from its name.
 *
 * @param	name		The name of the class of widget.
 *
 * @return			The structure describing the class.
 */
ei_widgetclass_t*	ei_widgetclass_from_name	(ei_widgetclass_name_t name){
    ei_widgetclass_t *current = &LIB;
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
}




/**
 * @brief	Configures the attributes of widgets of the class "frame".
 *
 *		Parameters obey the "default" protocol: if a parameter is "NULL" and it has never
 *		been defined before, then a default value should be used (default values are
 *		specified for each parameter). If the parameter is "NULL" but was defined on a
 *		previous call, then its value must not be changed.
 *
 * @param	widget		The widget to configure.
 * @param	requested_size	The size requested for this widget, including borders.
 *				The geometry manager may override this size due to other constraints.
 *				Defaults to the "natural size" of the widget, ie. big enough to
 *				display the border and the text or the image. This may be (0, 0)
 *				if the widget has border_width=0, and no text and no image.
 * @param	color		The color of the background of the widget. Defaults to
 *				\ref ei_default_background_color.
 * @param	border_width	The width in pixel of the border decoration of the widget. The final
 *				appearance depends on the "relief" parameter. Defaults to 0.
 * @param	relief		Appearance of the border of the widget. Defaults to
 *				\ref ei_relief_none.
 * @param	text		The text to display in the widget, or NULL. Only one of the
 *				parameter "text" and "img" should be used (i.e. non-NULL). Defaults
 *				to NULL.
 * @param	text_font	The font used to display the text. Defaults to \ref ei_default_font.
 * @param	text_color	The color used to display the text. Defaults to 
 *				\ref ei_font_default_color.
 * @param	text_anchor	The anchor of the text, i.e. where it is placed whithin the widget
 *				when the size of the widget is bigger than the size of the text.
 *				Defaults to \ref ei_anc_center.
 * @param	img		The image to display in the widget, or NULL. Any surface can be
 *				used, but usually a surface returned by \ref hw_image_load. Only one
 *				of the parameter "text" and "img" should be used (i.e. non-NULL).
 				Defaults to NULL.
 * @param	img_rect	If not NULL, this rectangle defines a subpart of "img" to use as the
 *				image displayed in the widget. Defaults to NULL.
 * @param	img_anchor	The anchor of the image, i.e. where it is placed whithin the widget
 *				when the size of the widget is bigger than the size of the image.
 *				Defaults to \ref ei_anc_center.
 */
void			ei_frame_configure		(ei_widget_t*		widget,
							 ei_size_t*		requested_size,
							 const ei_color_t*	color,
							 int*			border_width,
							 ei_relief_t*		relief,
							 char**			text,
							 ei_font_t*		text_font,
							 ei_color_t*		text_color,
							 ei_anchor_t*		text_anchor,
							 ei_surface_t*		img,
							 ei_rect_t**		img_rect,
							 ei_anchor_t*		img_anchor){
    if (requested_size != NULL){
       widget -> requested_size = *requested_size;
    }
    ei_frame_t* frame = (ei_frame_t*) widget;
    if (color != NULL){
        frame -> color = color;
    }
    if (border_width != NULL){
        frame -> border_width = border_width;
    }
    if (relief != NULL){
        frame -> relief = relief;
    }
    if (text != NULL){
        frame -> text = text;
    }
    if (text_font != NULL){
        frame -> text_font = text_font;
    }
    if (text_color != NULL){
        frame -> text_color = text_color;
    }
    if (img != NULL){
        frame -> img = img;
    }
    if (img_rect != NULL){
        frame -> img_rect = img_rect;
    }
    if (img_anchor != NULL){
        frame -> img_anchor = img_anchor;
    }
}






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
							 void**			user_param){
}

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
						 	 ei_size_t**		min_size){
}

/**
 * \brief	Registers the "frame" widget class in the program. This must be called only
 *		once before widgets of the class "frame" can be created and configured with
 *		\ref ei_frame_configure.
 */
void			ei_frame_register_class 	(){
    ei_widgetclass_t* frame = calloc(1, sizeof(ei_widgetclass_t));
    frame -> drawfunc = &ei_frame_drawfunc_t;
    frame -> handlefunc = &ei_frame_handlefunc_t;
    frame -> geomnotifyfunc = &ei_frame_geomnotifyfunc_t;
    frame -> releasefunc = &ei_frame_releasefunc_t;
    frame -> allocfunc = &ei_frame_allocfunc_t;
    frame -> setdefaultsfunc = &ei_frame_setdefaultsfunc_t;
    strncpy(frame -> name, "frame", 20);
    ei_widgetclass_register(frame);

}

/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a widget of a class. After allocation, the function *must*
 *		initialize the memory to 0.
 *
 * @return		A block of memory with all bytes set to 0.
 */
void*	ei_frame_allocfunc_t		(){
    return calloc(1, sizeof(ei_frame_t));
}

/**
 * \brief	A function that releases the memory used by a widget before it is destroyed.
 *		The \ref ei_widget_t structure itself, passed as parameter, must *not* by freed by
 *		these functions. Can be set to NULL in \ref ei_widgetclass_t if no memory is used by
 *		a class of widget.
 *
 * @param	widget		The widget which resources are to be freed.
 */

void	ei_frame_releasefunc_t	(struct ei_widget_t*	widget){
    ei_frame_t* frame = (ei_frame_t*) widget;
    free(widget -> wclass);
    free(frame);
}

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
							 ei_rect_t*		clipper){
    ei_frame_t* frame = (ei_frame_t*) widget;
    ei_fill(surface, frame -> color, clipper);
}


/**
 * \brief	A function that sets the default values for a class.
 *
 * @param	widget		A pointer to the widget instance to intialize.
 */
void	ei_frame_setdefaultsfunc_t	(struct ei_widget_t*	widget){
    ei_frame_t* frame = (ei_frame_t *) widget;
    ei_color_t* color = malloc(sizeof(ei_color_t));
    *color = ei_default_background_color;
    frame -> color = color;
    frame -> border_width = 0;
    frame -> relief = ei_relief_none;
    frame -> text_font = ei_default_font;
    frame -> text_color = &ei_font_default_color;
    ei_anchor_t* center_text = malloc(sizeof(ei_anchor_t));
    *center_text = ei_anc_center;
    ei_anchor_t* center_img = malloc(sizeof(ei_anchor_t));
    *center_img = ei_anc_center;
    frame -> text_anchor = center_text;
    frame -> img_anchor = center_img;


}

/**
 * \brief 	A function that is called to notify the widget that its geometry has been modified
 *		by its geometry manager. Can set to NULL in \ref ei_widgetclass_t.
 *
 * @param	widget		The widget instance to notify of a geometry change.
 * @param	rect		The new rectangular screen location of the widget
 *				(i.e. = widget->screen_location).
 */

void	ei_frame_geomnotifyfunc_t	(struct ei_widget_t*	widget,
							 ei_rect_t		rect){
}
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
						 struct ei_event_t*	event){
}


/**
 * @brief	Registers a class to the program so that widgets of this class can be created.
 *		This must be done only once per widged class in the application.
 *
 * @param	widgetclass	The structure describing the class.
 */
void			ei_widgetclass_register		(ei_widgetclass_t* widgetclass){
    ei_widgetclass_t* current = &LIB;
    while (current -> next != NULL){
        current = current -> next;
    }
    current -> next = widgetclass;
}
