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
#include "ei_arc.h"
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
uint32_t COLOR_ID = 1;

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
    if (requested_size != NULL){
        widget -> requested_size = *requested_size;
    } else {
        if (text != NULL){
            ei_surface_t text_surface = hw_text_create_surface(*(frame -> text), *(frame -> text_font), (frame -> text_color));
            ei_size_t text_size = hw_surface_get_size(text_surface);
            (widget -> requested_size).width = text_size.width + *(frame -> border_width)*2 + 5;
            (widget -> requested_size).height = text_size.height + *(frame -> border_width)*2 + 5;
        } else if (img != NULL) {
            ei_size_t size = (**(frame -> img_rect)).size;
            (widget -> requested_size).width = size.width + *(frame -> border_width)*2 + 5;
            (widget -> requested_size).height = size.height + *(frame -> border_width)*2 + 5;
                    }
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

                ei_button_t* button = (ei_button_t*) widget;

				 if (relief != NULL) {
					button -> relief = relief;
				 }
				 if (border_width != NULL) {
					 button -> border_width = border_width;
				 }
				 if (corner_radius != NULL) {
					 button -> corner_radius = corner_radius;
				 }
				 if (callback != NULL) {
				 	button -> callback = callback;
				 }
				 if (user_param != NULL){
					 button -> user_param = user_param;
				 }
				 if (img_rect != NULL){
						 button -> img_rect = img_rect;
				 }
				 if (img_anchor != NULL){
						 button -> img_anchor = img_anchor;
				 }
				 if (img != NULL){
							button -> img = img;
					}
				 if (text_anchor != NULL){
					 button -> text_anchor = text_anchor;
				 }
				 if (text_color != NULL){
		 			button -> text_color = text_color;
		 		}
				if (text_font != NULL){
					button -> text_font = text_font;
				}
				if (text != NULL) {
					button -> text = text;
				}
				if (color != NULL) {
					button -> color = color;
				}
				if (requested_size != NULL){
					 button -> requested_size = requested_size;
				}
                if (requested_size != NULL){
                    widget -> requested_size = *requested_size;
                } else {
                    if (text != NULL){
                        ei_surface_t text_surface = hw_text_create_surface(*(button -> text), *(button -> text_font), (button -> text_color));
                        ei_size_t text_size = hw_surface_get_size(text_surface);
                        (widget -> requested_size).width = text_size.width + *(button -> border_width)*2;
                        (widget -> requested_size).height = text_size.height + *(button -> border_width)*2;
                    } else if (img != NULL) {
                        ei_size_t size = (**(button -> img_rect)).size;
                        (widget -> requested_size).width = size.width + *(button -> border_width)*2;
                        (widget -> requested_size).height = size.height + *(button -> border_width)*2;
                    }

}
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
	 if (requested_size != NULL){
			 widget -> requested_size = *requested_size;
	 }
	 ei_toplevel_t* toplevel = (ei_toplevel_t*) widget;
	 if (color != NULL){
			 toplevel -> color = color;
	 }
	 if (border_width != NULL){
			 toplevel -> border_width = border_width;
	 }
	 if (title != NULL){
			 toplevel -> title = title;
	 }
	 if (closable != NULL){
			 toplevel -> closable = closable;
	 }
	 if (resizable != NULL){
			 toplevel -> resizable = resizable;
	 }
	 if (min_size != NULL){
			 toplevel -> min_size = min_size;
	 }
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
 * \brief	Registers the "toplevel" widget class in the program. This must be called only
 *		once before widgets of the class "toplevel" can be created and configured with
 *		\ref ei_toplevel_configure.
 */
void			ei_toplevel_register_class 	(){
    ei_widgetclass_t* toplevel = calloc(1, sizeof(ei_widgetclass_t));
    toplevel -> drawfunc = &ei_toplevel_drawfunc_t;
    toplevel -> handlefunc = &ei_toplevel_handlefunc_t;
    toplevel -> geomnotifyfunc = &ei_toplevel_geomnotifyfunc_t;
    toplevel -> releasefunc = &ei_toplevel_releasefunc_t;
    toplevel -> allocfunc = &ei_toplevel_allocfunc_t;
    toplevel -> setdefaultsfunc = &ei_toplevel_setdefaultsfunc_t;
    strncpy(toplevel -> name, "toplevel", 20);
    ei_widgetclass_register(toplevel);
}

/**
 * \brief	Registers the "button" widget class in the program. This must be called only
 *		once before widgets of the class "button" can be created and configured with
 *		\ref ei_button_configure.
 */
void			ei_button_register_class 	(){
    ei_widgetclass_t* button = calloc(1, sizeof(ei_widgetclass_t));
    button -> drawfunc = &ei_button_drawfunc_t;
    button -> handlefunc = &ei_button_handlefunc_t;
    button -> geomnotifyfunc = &ei_button_geomnotifyfunc_t;
    button -> releasefunc = &ei_button_releasefunc_t;
    button -> allocfunc = &ei_button_allocfunc_t;
    button -> setdefaultsfunc = &ei_button_setdefaultsfunc_t;
    strncpy(button -> name, "button", 20);
    ei_widgetclass_register(button);
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
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a widget of a class. After allocation, the function *must*
 *		initialize the memory to 0.
 *
 * @return		A block of memory with all bytes set to 0.
 */
void*	ei_toplevel_allocfunc_t		(){
		return calloc(1, sizeof(ei_toplevel_t));
}

/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a widget of a class. After allocation, the function *must*
 *		initialize the memory to 0.
 *
 * @return		A block of memory with all bytes set to 0.
 */
void*	ei_button_allocfunc_t		(){
		return calloc(1, sizeof(ei_button_t));
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
 * \brief	A function that releases the memory used by a widget before it is destroyed.
 *		The \ref ei_widget_t structure itself, passed as parameter, must *not* by freed by
 *		these functions. Can be set to NULL in \ref ei_widgetclass_t if no memory is used by
 *		a class of widget.
 *
 * @param	widget		The widget which resources are to be freed.
 */

void	ei_toplevel_releasefunc_t	(struct ei_widget_t*	widget){
    ei_toplevel_t* toplevel = (ei_toplevel_t*) widget;
    free(widget -> wclass);
    free(toplevel);
}

/**
 * \brief	A function that releases the memory used by a widget before it is destroyed.
 *		The \ref ei_widget_t structure itself, passed as parameter, must *not* by freed by
 *		these functions. Can be set to NULL in \ref ei_widgetclass_t if no memory is used by
 *		a class of widget.
 *
 * @param	widget		The widget which resources are to be freed.
 */

void	ei_button_releasefunc_t	(struct ei_widget_t*	widget){
    ei_button_t* button = (ei_button_t*) widget;
    free(widget -> wclass);
    free(button);
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
	ei_rect_t rectangle = widget -> screen_location;
	ei_color_t color = *(frame -> color);
	int border_width = *(frame -> border_width);
	ei_relief_t relief = *(frame -> relief);
	char** text = frame -> text;
	ei_surface_t* img = frame -> img;
	ei_rect_t** img_rect = calloc(1, sizeof(ei_rect_t));
	ei_font_t* text_font = calloc(1, sizeof(ei_font_t));
	ei_color_t* text_color = calloc(1, sizeof(ei_color_t));
	ei_anchor_t *anchor = calloc(1, sizeof(ei_anchor_t));
	ei_point_t* where = calloc(1, sizeof(ei_point_t));
	if (img != NULL) {
		img_rect = frame -> img_rect;
		anchor = frame -> img_anchor;
		where = ei_get_where(rectangle, anchor, border_width, (**img_rect).size);
	} else if (text != NULL) {
		text_font = frame -> text_font;
		text_color = frame -> text_color;
		anchor = frame -> text_anchor;
		ei_surface_t text_surface = hw_text_create_surface(*text, *text_font, &color);
		ei_size_t text_size = hw_surface_get_size(text_surface);
		where = ei_get_where(rectangle, anchor, border_width, text_size);
	}
	ei_draw_button(surface, rectangle, color, 0, border_width, relief, text, *text_font, text_color, img, *img_rect, *where, clipper);
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
void	ei_button_drawfunc_t		(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
                             ei_surface_t		pick_surface,
							 ei_rect_t*		clipper){
    ei_button_t* button = (ei_button_t*) widget;
	ei_rect_t rectangle = widget -> screen_location;
	ei_color_t color = *(button -> color);
	int border_width = *(button -> border_width);
	ei_relief_t relief = *(button -> relief);
	char** text = button -> text;
	ei_surface_t* img = button -> img;
	ei_rect_t** img_rect = calloc(1, sizeof(ei_rect_t));
	ei_font_t* text_font = calloc(1, sizeof(ei_font_t));
	ei_color_t* text_color = calloc(1, sizeof(ei_color_t));
	ei_anchor_t *anchor = calloc(1, sizeof(ei_anchor_t));
	ei_point_t* where = calloc(1, sizeof(ei_point_t));
	if (img != NULL) {
		img_rect = button -> img_rect;
		anchor = button -> img_anchor;
		where = ei_get_where(rectangle, anchor, border_width, (**img_rect).size);
	} else if (text != NULL) {
		text_font = button -> text_font;
		text_color = button -> text_color;
		anchor = button -> text_anchor;
		ei_surface_t text_surface = hw_text_create_surface(*text, *text_font, &color);
		ei_size_t text_size = hw_surface_get_size(text_surface);
		where = ei_get_where(rectangle, anchor, border_width, text_size);
	}
	ei_draw_button(surface, rectangle, color, *(button -> corner_radius), border_width, relief, text, *text_font, text_color, img, *img_rect, *where, clipper);
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
ei_point_t* ei_get_where(ei_rect_t rectangle, ei_anchor_t* anchor, int border_width, ei_size_t size) {
	ei_point_t* where = calloc(1, sizeof(ei_point_t));
	ei_point_t point_ancre = {rectangle.top_left.x + rectangle.size.width/2, rectangle.top_left.y + rectangle.size.height/2};
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
            where -> y = point_ancre.y + (rectangle.size.height/2) - size.height - border_width;
            break;
        case ei_anc_south:
            where -> x = point_ancre.x - 0.5 * size.width;
            where -> y = point_ancre.y + (rectangle.size.height/2) - size.height - border_width;
            break;
        case ei_anc_southeast:
            where -> x = point_ancre.x + (rectangle.size.width/2)- size.width - border_width;
            where -> y = point_ancre.y + (rectangle.size.height/2) - size.height - border_width;
            break;
        case ei_anc_east:
            where -> x = point_ancre.x + (rectangle.size.width/2)- size.width - border_width;
            where -> y = point_ancre.y - 0.5 * size.height;
            break;
        case ei_anc_northeast:
            where -> x = point_ancre.x + (rectangle.size.width/2)- size.width - border_width;
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
							 ei_rect_t*		clipper) {
	 ei_toplevel_t* toplevel = (ei_toplevel_t*) widget;
	 ei_rect_t rectangle = widget -> screen_location;
	 ei_color_t* color = toplevel -> color;
	 int* border_width = toplevel -> border_width;
	 char** title = toplevel -> title;
	 ei_bool_t* closable = toplevel -> closable;
	 ei_axis_set_t* resizable = toplevel -> resizable;
	 ei_draw_toplevel(surface, rectangle, color, *border_width, title, clipper);
 }

/**
 * \brief	A function that sets the default values for a class.
 *
 * @param	widget		A pointer to the widget instance to intialize.
 */
void	ei_frame_setdefaultsfunc_t	(struct ei_widget_t*	widget){
    (widget -> requested_size).width = 100;
    (widget -> requested_size).height = 100;
    ei_frame_t* frame = (ei_frame_t *) widget;
    ei_color_t* color = malloc(sizeof(ei_color_t));
    *color = ei_default_background_color;
    frame -> color = color;
	int* border_width = calloc(1, sizeof(int));
    frame -> border_width = border_width;
	ei_relief_t *relief = malloc(sizeof(ei_relief_t));
	*relief = ei_relief_none;
    frame -> relief = relief;
	ei_font_t* text_font = malloc(sizeof(ei_font_t));
	*text_font = ei_default_font;
    frame -> text_font = text_font;
	ei_color_t* text_color = malloc(sizeof(ei_color_t));
	*text_color = ei_font_default_color;
    frame -> text_color = text_color;
    ei_anchor_t* center_text = malloc(sizeof(ei_anchor_t));
    *center_text = ei_anc_center;
    ei_anchor_t* center_img = malloc(sizeof(ei_anchor_t));
    *center_img = ei_anc_center;
    frame -> text_anchor = center_text;
    frame -> img_anchor = center_img;
}

/**
 * \brief	A function that sets the default values for a class.
 *
 * @param	widget		A pointer to the widget instance to intialize.
 */
void	ei_button_setdefaultsfunc_t	(struct ei_widget_t*	widget){
    ei_button_t* button = (ei_button_t *) widget;
    ei_color_t* color = malloc(sizeof(ei_color_t));
    *color = ei_default_background_color;
    button -> color = color;
	int* border_width = calloc(1, sizeof(int));
    button -> border_width = border_width;
	ei_relief_t *relief = malloc(sizeof(ei_relief_t));
	*relief = ei_relief_raised;
    button -> relief = relief;
	ei_font_t* text_font = malloc(sizeof(ei_font_t));
	*text_font = ei_default_font;
    button -> text_font = text_font;
	ei_color_t* text_color = malloc(sizeof(ei_color_t));
	*text_color = ei_font_default_color;
    button -> text_color = text_color;
    ei_anchor_t* center_text = malloc(sizeof(ei_anchor_t));
    *center_text = ei_anc_center;
    ei_anchor_t* center_img = malloc(sizeof(ei_anchor_t));
    *center_img = ei_anc_center;
    button -> text_anchor = center_text;
    button -> img_anchor = center_img;
    int* corner = malloc(sizeof(int));
    *corner = k_default_button_corner_radius;
    button -> corner_radius = corner;

}
/**
* \brief	A function that sets the default values for a class.
*
* @param	widget		A pointer to the widget instance to intialize.
*/
void	ei_toplevel_setdefaultsfunc_t	(struct ei_widget_t*	widget) {
		(widget -> requested_size).width = 320;
		(widget -> requested_size).height = 240;
		ei_toplevel_t* toplevel = (ei_toplevel_t *) widget;
		ei_color_t* color = malloc(sizeof(ei_color_t));
		*color = ei_default_background_color;
		toplevel -> color = color;
		int* border_width = calloc(1, sizeof(int));
		toplevel -> border_width = border_width;
		char* title = "Toplevel";
		toplevel -> title = &title;
		ei_bool_t* closable = malloc(sizeof(ei_bool_t));
		*closable = EI_TRUE;
		toplevel -> closable = closable;
		ei_axis_set_t* resizable = malloc(sizeof(ei_axis_set_t));
		*resizable = ei_axis_both;
		toplevel -> resizable = resizable;
		ei_size_t* min_size = malloc(sizeof(ei_size_t));
		min_size -> width = 160;
		min_size -> height = 120;
		toplevel -> min_size = &min_size;
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
 * \brief 	A function that is called to notify the widget that its geometry has been modified
 *		by its geometry manager. Can set to NULL in \ref ei_widgetclass_t.
 *
 * @param	widget		The widget instance to notify of a geometry change.
 * @param	rect		The new rectangular screen location of the widget
 *				(i.e. = widget->screen_location).
 */

void	ei_toplevel_geomnotifyfunc_t	(struct ei_widget_t*	widget,
							 ei_rect_t		rect){
}

/**
 * \brief 	A function that is called to notify the widget that its geometry has been modified
 *		by its geometry manager. Can set to NULL in \ref ei_widgetclass_t.
 *
 * @param	widget		The widget instance to notify of a geometry change.
 * @param	rect		The new rectangular screen location of the widget
 *				(i.e. = widget->screen_location).
 */

void	ei_button_geomnotifyfunc_t	(struct ei_widget_t*	widget,
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
						 struct ei_event_t*	event){
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
ei_bool_t ei_button_handlefunc_t (struct ei_widget_t*	widget,
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
