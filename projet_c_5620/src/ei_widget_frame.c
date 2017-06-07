#include "ei_all_widgets.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ei_draw_widgets.h"
#include "ei_draw_extension.h"
#include "ei_application.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "ei_widget_frame.h"
#include "ei_widget_button.h"
#include "ei_widget_toplevel.h"

#define max(a,b) ((a) > (b) ? a : b)
#define min(a,b) ((a) < (b) ? a : b)
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
    ei_frame_t* frame = (ei_frame_t*) widget;
    if (color != NULL){
        *(frame -> color) = *color;
    }
    if (border_width != NULL){
        *(frame -> border_width) = *border_width;
    }
    if (relief != NULL){
        *(frame -> relief) = *relief;
    }
    if (text != NULL){
        frame -> text = calloc(1, sizeof(char*));
        *(frame -> text) = calloc(strlen(*text), sizeof(char));
        strncpy(*(frame -> text), *text, strlen(*text));
    }
    if (text_font != NULL){
        *(frame -> text_font) = *text_font;
    }
    if (text_color != NULL){
        *(frame -> text_color) = *text_color;
    }
    if (img != NULL){
        frame -> img = malloc(sizeof(ei_surface_t));
        *(frame -> img) = *img;
    }
    if (img_rect != NULL){
        frame -> img_rect = calloc(1, sizeof(ei_rect_t*));
        *(frame -> img_rect) = calloc(1, sizeof(ei_rect_t));
        **(frame -> img_rect) = **img_rect;
    }
    if (img_anchor != NULL){
        frame -> img_anchor = malloc(sizeof(ei_anchor_t));
        *(frame -> img_anchor) = *img_anchor;
    }
    if (requested_size != NULL){
        widget -> requested_size = *requested_size;
    } else {
        if (text != NULL){
            ei_surface_t text_surface = hw_text_create_surface(*(frame -> text),
             *(frame -> text_font), (frame -> text_color));
            ei_size_t text_size = hw_surface_get_size(text_surface);
            (widget -> requested_size).width = text_size.width +
            *(frame -> border_width)*2 + 5;
            (widget -> requested_size).height = text_size.height +
            *(frame -> border_width)*2 + 5;
        } else if (img != NULL) {
            ei_size_t size = (**(frame -> img_rect)).size;
            (widget -> requested_size).width = size.width +
            *(frame -> border_width)*2 + 5;
            (widget -> requested_size).height = size.height +
            *(frame -> border_width)*2 + 5;
        }
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
    free(frame -> color);
    free(frame -> relief);
    free(frame -> text_font);
    free(frame -> text_color);
    free(frame -> text_anchor);
    free(frame -> img_anchor);
    if (frame -> text != NULL){
        free(*(frame -> text));
        free(frame -> text);
    }
    if (frame -> img != NULL){
        free(frame -> img);
    }
    if (frame -> img_rect != NULL){
        free(*(frame -> img_rect));
        free(frame -> img_rect);
    }

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
    ei_rect_t rectangle = widget -> screen_location;
    ei_color_t color = *(frame -> color);
    ei_color_t pick_color = *(widget -> pick_color);
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
        text_color = (ei_color_t*) frame -> text_color;
        anchor = frame -> text_anchor;
        ei_surface_t text_surface = hw_text_create_surface(*text, *text_font, &color);
        ei_size_t text_size = hw_surface_get_size(text_surface);
        where = ei_get_where(rectangle, anchor, border_width, text_size);
    }
    ei_size_t surface_size = hw_surface_get_size(surface);
    if (color.alpha != 255){
        ei_surface_t alpha_surface = hw_surface_create(surface, &surface_size,
            EI_TRUE);
        ei_draw_button(alpha_surface, rectangle, color, 0, border_width, relief,
             text, *text_font, text_color, img, *img_rect, *where, clipper);
        ei_copy_surface(surface,clipper, alpha_surface,clipper, EI_TRUE);
    } else {
        ei_draw_button(surface, rectangle, color, 0, border_width, relief, text,
             *text_font, text_color, img, *img_rect, *where, clipper);
    }

    if (text != NULL) {
        if (*text != NULL) {
            rectangle.size.width -= 2*border_width;
            rectangle.size.height -= 2*border_width;
            rectangle.top_left.x += border_width;
            rectangle.top_left.y += border_width;
            rectangle = *ei_intersection(&rectangle, clipper);
            ei_draw_text(surface, where, *text, *text_font, text_color, &rectangle);
        }
    } else if (img != NULL) {
        // on utilise copy
        ei_rect_t rect = {*where, (*img_rect)->size};
        ei_rect_t image = **img_rect;
        ei_rect_t* intersects = &rect;
        rectangle.size.width -= 2*border_width;
        rectangle.size.height -= 2*border_width;
        rectangle.top_left.x += border_width;
        rectangle.top_left.y += border_width;
        clipper = ei_intersection(&rectangle, clipper);
        if (clipper != NULL) {
            if (clipper->size.width != 0 && clipper->size.height != 0) {
                intersects = ei_intersection(&rect, clipper);
                image.top_left.x += max(0,intersects ->top_left.x - where->x);
                image.top_left.y += max(0,intersects ->top_left.y - where->y);
                image.size = intersects -> size;
            }
        }
        if (image.size.width != 0 && image.size.height != 0) {
            ei_size_t surface_size = hw_surface_get_size(surface);
            if (intersects -> top_left.x >= 0 && intersects -> top_left.y >= 0
                && intersects -> top_left.x <= surface_size.width
                && intersects -> top_left.y <= surface_size.height) {
                ei_copy_surface(surface, intersects, *img, &image,
                     hw_surface_has_alpha(surface));
            }
        }
    }
    ei_draw_button(pick_surface, rectangle, pick_color, 0, 0, ei_relief_none,
        NULL, ei_default_font, &color, NULL, &rectangle, *where, clipper);
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
    ei_event_set_active_widget(widget -> parent);
    return EI_FALSE;
}
