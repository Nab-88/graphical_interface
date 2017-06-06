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

/*
 * button_press --
 *
 *	Callback called when a user clicks on the button.
 */
void button_press2(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
    if (DRAW_RECT == NULL){
        DRAW_RECT = calloc(1, sizeof(ei_linked_rect_t));
    }
    DRAW_RECT -> rect = widget -> parent -> screen_location;
    ei_widget_destroy(widget -> parent);
    ei_event_set_active_widget(NULL);
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
        *(button -> relief) = *relief;
    }
    if (border_width != NULL) {
        *(button -> border_width) = *border_width;
    }
    if (corner_radius != NULL) {
        *(button -> corner_radius) = *corner_radius;
    }
    if (callback != NULL) {
        button -> callback = malloc(sizeof(ei_callback_t));
        *(button -> callback) = *callback;
    }
    if (user_param != NULL){
        *(button -> user_param) = *user_param;
    }
    if (img_rect != NULL){
        button -> img_rect = calloc(1, sizeof(ei_rect_t*));
        *(button -> img_rect) = calloc(1, sizeof(ei_rect_t));
        **(button -> img_rect) = **img_rect;
    }
    if (img_anchor != NULL){
        button -> img_anchor = calloc(1,sizeof(ei_anchor_t));
        *(button -> img_anchor) = *img_anchor;
    }
    if (img != NULL){
        button -> img = calloc(1, sizeof(ei_rect_t));
        *(button -> img) = *img;
    }
    if (text_anchor != NULL){
        //button -> text_anchor = calloc(1, sizeof(ei_rect_t));
        *(button -> text_anchor) = *text_anchor;
    }
    if (text_color != NULL){
        *(button -> text_color) = *text_color;
    }
    if (text_font != NULL){
        *(button -> text_font) = *text_font;
    }
    if (text != NULL) {
        button -> text = calloc(1, sizeof(char*));
        *(button -> text) = calloc(strlen(*text), sizeof(char));
        strncpy(*(button -> text), *text, strlen(*text));
        //*(button -> text) = *text;
    }
    if (color != NULL) {
        //button -> color = calloc(1, sizeof(ei_color_t));
        *(button -> color) = *color;
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

void	ei_button_releasefunc_t	(struct ei_widget_t*	widget){
    ei_button_t* button = (ei_button_t*) widget;
    free(button -> color);
    free(button -> user_param);
    free(button -> border_width);
    free(button -> relief);
    free(button -> text_font);
    free(button -> text_color);
    free(button -> text_anchor);
    free(button -> img_anchor);
    free(button -> corner_radius);
    if (button -> callback != NULL){
        free(button -> callback);
    }
    if (button -> img_rect != NULL){
        free(*(button -> img_rect));
        free(button -> img_rect);
    }
    if (button -> img != NULL){
        free(button -> img);
    }
    if (button -> text != NULL) {
        free(*(button -> text));
        free(button -> text);
    }

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
void	ei_button_drawfunc_t		(struct ei_widget_t*	widget,
        ei_surface_t		surface,
        ei_surface_t		pick_surface,
        ei_rect_t*		clipper){
    ei_button_t* button = (ei_button_t*) widget;
    ei_rect_t rectangle = widget -> screen_location;
    ei_color_t color = *(button -> color);
    ei_color_t pick_color = *(widget -> pick_color);
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
    ei_draw_button(pick_surface, rectangle, pick_color, *(button -> corner_radius), 0, ei_relief_none, NULL, ei_default_font, &color, NULL, &rectangle, *where, clipper);
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
    button -> user_param = calloc(1, sizeof(void *));
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
ei_bool_t ei_button_handlefunc_t (struct ei_widget_t*	widget,
        struct ei_event_t*	event){
    ei_button_t* button = (ei_button_t*) widget;
    ei_relief_t* relief = button -> relief;
    if (event -> type == ei_ev_mouse_buttondown) {
        *relief = ei_relief_sunken;
        ei_change_relief_button(relief, widget);
        if (DRAW_RECT == NULL){
            DRAW_RECT = calloc(1, sizeof(ei_linked_rect_t));
        }
        DRAW_RECT -> rect = widget -> screen_location;
        ei_event_set_active_widget(widget);
    }
    else if (event -> type == ei_ev_mouse_buttonup) {
        *relief = ei_relief_raised;
        ei_change_relief_button(relief, widget);
        if (DRAW_RECT == NULL){
            DRAW_RECT = calloc(1, sizeof(ei_linked_rect_t));
        }
        DRAW_RECT -> rect = widget -> screen_location;
        ei_event_set_active_widget(widget -> parent);
        if (button -> callback != NULL){
            (*(button -> callback))(widget, event, *(button -> user_param));
        }
    }
    else if (event -> type == ei_ev_mouse_move) {
        ei_point_t where = event -> param.mouse.where;
        ei_widget_t* compare = ei_widget_pick(&where);
        if (compare == NULL || compare -> pick_id != widget -> pick_id) {
            if (*relief != ei_relief_raised) {
                *relief = ei_relief_raised;
                ei_change_relief_button(relief, widget);
            }
        }
        else {
            if (*relief != ei_relief_sunken) {
                *relief = ei_relief_sunken;
                ei_change_relief_button(relief, widget);
            }
        }
    }
    return EI_TRUE;
}

/**
 * @brief	Changes the relief of a button
 *
 * @param	relief	The relief that we change to
 * @param   widget the current widget
 */
void ei_change_relief_button(ei_relief_t* relief, ei_widget_t* widget) {
    ei_button_configure(widget, NULL, NULL,NULL,NULL, relief, NULL,NULL,NULL,NULL,NULL, NULL,NULL,NULL,NULL);
    ei_placer_run(widget);
}
