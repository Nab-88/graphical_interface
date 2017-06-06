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
        requested_size -> height += 30;
        widget -> requested_size = *requested_size;
    }
    ei_toplevel_t* toplevel = (ei_toplevel_t*) widget;
    if (color != NULL){
        *(toplevel -> color) = *color;
    }
    if (border_width != NULL){
        *(toplevel -> border_width) = *border_width;
    }
    if (title != NULL){
        toplevel -> title = calloc(1, sizeof(char*));
        *(toplevel -> title) = calloc(strlen(*title), sizeof(char));
        strncpy(*(toplevel -> title), *title, strlen(*title));
    }
    if (closable != NULL){
        *(toplevel -> closable) = *closable;
    }
    if (resizable != NULL){
        *(toplevel -> resizable) = *resizable;
    }
    if (min_size != NULL){
        *(toplevel -> min_size) = *min_size;
    }
    toplevel -> button_closable = NULL;
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
 * \brief	A function that releases the memory used by a widget before it is destroyed.
 *		The \ref ei_widget_t structure itself, passed as parameter, must *not* by freed by
 *		these functions. Can be set to NULL in \ref ei_widgetclass_t if no memory is used by
 *		a class of widget.
 *
 * @param	widget		The widget which resources are to be freed.
 */

void	ei_toplevel_releasefunc_t	(struct ei_widget_t*	widget){
    ei_toplevel_t* toplevel = (ei_toplevel_t*) widget;
    free(toplevel -> color);
    free(toplevel -> border_width);
    free(toplevel -> closable);
    free(toplevel -> resizable);
    free(*(toplevel -> min_size));
    free(toplevel -> min_size);
    if (toplevel -> title != NULL){
        free(*(toplevel -> title));
        free(toplevel -> title);
    }
    free(toplevel);
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
    ei_color_t* pick_color = widget -> pick_color;
    int* border_width = toplevel -> border_width;
    char** title = toplevel -> title;
    ei_axis_set_t* resizable = toplevel -> resizable;
    ei_color_t window_color = {110, 110, 110, 255};
    ei_size_t surface_size = hw_surface_get_size(surface);
    ei_surface_t alpha_surface = hw_surface_create(surface, &surface_size, EI_TRUE);
    ei_draw_toplevel(alpha_surface, rectangle, color, &window_color, *border_width, title, clipper);
    ei_copy_surface(surface,clipper, alpha_surface,clipper, EI_TRUE);
    ei_color_t text_color = {0, 0, 0, 255};
    ei_point_t* where = calloc(1, sizeof(ei_point_t));
    where -> x = rectangle.top_left.x + 25; // on garde de la place pour le bouton closable
    where -> y = rectangle.top_left.y;
    ei_draw_text(surface, where, *title, ei_default_font, &text_color, clipper);
    free(where);
    ei_draw_toplevel(pick_surface, rectangle, pick_color, pick_color, 0, NULL, clipper);
    if (*(toplevel -> closable) == EI_TRUE && toplevel -> button_closable == NULL) {
        ei_point_t point;
        ei_size_t size;
        point.x = 10;
        point.y = 10;
        ei_anchor_t anchor = ei_anc_none;
        size.width = 16;
        size.height = 16;
        ei_widget_t* closable = ei_widget_create("button", widget);
        ei_color_t* red = calloc(1, sizeof(ei_color_t));
        red -> red = 255;
        red -> alpha = 255;
        int width = 3;
        int radius = 10;
        ei_callback_t callback = button_press2;
        ei_button_configure(closable, &size, red, &width, &radius, NULL, NULL, NULL, red, NULL,NULL,NULL,NULL, &callback, NULL);
        ei_place(closable, &anchor, &(point.x), &(point.y), NULL, NULL, NULL, NULL, NULL, NULL);
        toplevel -> button_closable = closable;
    }
    if (*resizable != ei_axis_none) {
        ei_rect_t* rect_resiz = malloc(sizeof(ei_rect_t));
        rect_resiz ->top_left.x = rectangle.top_left.x + rectangle.size.width - 10 - *border_width;
        rect_resiz ->top_left.y = rectangle.top_left.y + rectangle.size.height - 10 - *border_width;
        rect_resiz ->size.width = 10 + *border_width;
        rect_resiz ->size.height = 10 + *border_width;
        ei_point_t* where = calloc(1, sizeof(ei_point_t));
        *where = (ei_point_t) {0, 0};
        ei_draw_button(surface, *rect_resiz, window_color, 0, 0, ei_relief_none, NULL, ei_default_font, &window_color, NULL, rect_resiz, *where, clipper);
        ei_draw_button(pick_surface, *rect_resiz, *pick_color, 0, 0, ei_relief_none, NULL, ei_default_font, color, NULL, rect_resiz, *where, clipper);
    }
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
    toplevel -> min_size = malloc(sizeof(ei_size_t*));
    *(toplevel -> min_size) = min_size;
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
    ei_toplevel_t* toplevel = (ei_toplevel_t*) widget;
    ei_point_t where = event -> param.mouse.where;
    if (event -> type == ei_ev_mouse_buttondown) {
        ei_event_set_active_widget(widget);
        if  (is_on_the_banner(widget, event) == EI_TRUE) {
            *WIN_MOVE = where;
        } else if (*(toplevel -> resizable) != ei_axis_none) {
            if (is_on_the_square(widget, event) == EI_TRUE) {
                switch (*(toplevel -> resizable)) {
                    case ei_axis_both:
                        *WIN_RESIZ = where;
                        break;
                    case ei_axis_x:
                        *WIN_RESIZ = (ei_point_t) {where.x, 0};
                        break;
                    case ei_axis_y:
                        *WIN_RESIZ = (ei_point_t) {0, where.y};
                        break;
                    default:
                        break;
                }
            }
        }
    }
    else if (event -> type == ei_ev_mouse_buttonup) {
        WIN_MOVE -> x = 0;
        WIN_MOVE -> y = 0;
        WIN_RESIZ -> x = 0;
        WIN_RESIZ -> y = 0;
        ei_event_set_active_widget(widget);
    }
    else if (event -> type == ei_ev_mouse_move) {
        if (WIN_MOVE -> x + WIN_MOVE -> y != 0) {

            if (DRAW_RECT == NULL){
                DRAW_RECT = malloc(sizeof(ei_linked_rect_t));
            }
            DRAW_RECT -> rect = (widget -> screen_location);
            int dx = where.x - WIN_MOVE -> x;
            int dy = where.y - WIN_MOVE -> y;
            int* x = calloc(1, sizeof(int));
            *x = widget -> screen_location.top_left.x + dx;
            int* y = calloc(1, sizeof(int));
            *y = widget -> screen_location.top_left.y + dy;
            int width = widget -> screen_location.size.width;
            int height = widget -> screen_location.size.height;
            ei_point_t point = {*x, *y};
            ei_size_t size = {width, height};
            ei_rect_t rectangle = {point, size};
            DRAW_RECT -> rect = *(ei_union(&(DRAW_RECT -> rect), &rectangle));

            ei_place(widget, NULL, x, y, &width, &height, NULL, NULL,NULL,NULL);
            *WIN_MOVE = where;

        }
        if (WIN_RESIZ -> x + WIN_RESIZ -> y != 0) {
            if (DRAW_RECT == NULL){
                DRAW_RECT = malloc(sizeof(ei_linked_rect_t));
            }
            DRAW_RECT -> rect = (widget -> screen_location);
            int dx, dy;
            if (WIN_RESIZ -> x != 0) {
                dx = where.x - WIN_RESIZ -> x;
                WIN_RESIZ -> x = where.x;
            } else {
                dx = 0;
            }
            if (WIN_RESIZ -> y != 0) {
                dy = where.y - WIN_RESIZ -> y;
                WIN_RESIZ -> y = where.y;
            } else {
                dy = 0;
            }
            int* x = calloc(1, sizeof(int));
            *x = widget -> screen_location.top_left.x;
            int* y = calloc(1, sizeof(int));
            *y = widget -> screen_location.top_left.y;
            int width = widget -> screen_location.size.width + dx;
            int height = widget -> screen_location.size.height + dy;
            ei_point_t point = {*x, *y};
            ei_size_t size = {width, height};
            ei_rect_t rectangle = {point, size};
            DRAW_RECT -> rect = *(ei_union(&(DRAW_RECT -> rect), &rectangle));
            if (width < ((*(toplevel -> min_size)) -> width)) {
                width = (*(toplevel -> min_size)) -> width;
            }
            if (height < ((*(toplevel -> min_size)) -> height)) {
                height = (*(toplevel -> min_size)) -> height;
            }
            ei_place(widget, NULL, x, y, &width, &height, NULL, NULL,NULL,NULL);
            ei_placer_run(widget);
        }
    }
    return EI_TRUE;
}
