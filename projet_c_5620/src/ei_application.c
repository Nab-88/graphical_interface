#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "ei_types.h"
#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_application.h"
#include "ei_all_widgets.h"
#include "ei_event.h"
#include "ei_widgetclass.h"

#define max(a,b) ((a) > (b) ? a : b)
#define min(a,b) ((a) < (b) ? a : b)

/**
 * \brief	Creates an application.
 *		<ul>
 *			<li> initializes the hardware (calls \ref hw_init), </li>
 *			<li> registers all classes of widget and all geometry managers, </li>
 *			<li> creates the root window (either in a system window, or the entire
 *				screen), </li>
 *			<li> creates the root widget to accress the root window. </li>
 *		</ul>
 *
 * @param	main_window_size	If "fullscreen is false, the size of the root window of the
 *					application.
 *					If "fullscreen" is true, the current monitor resolution is
 *					used as the size of the root window, this size is returned
 *					in this parameter.
 * @param	fullScreen		If true, the root window is the entire screen. Otherwise, it
 *					is a system window.
 */
void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen){
    // Init access to hardware
    hw_init();


    //Create the main window
    ei_surface_t main_window = hw_create_window(main_window_size, fullscreen);
    ei_surface_t pick_surface = hw_surface_create(main_window, main_window_size, EI_TRUE);
    LIB = calloc(1, sizeof(ei_widgetclass_t));
    ei_frame_register_class();
    ei_button_register_class();
    ei_toplevel_register_class();
    ei_widgetclass_name_t name;
    strncpy(name, "frame", 20);
    ei_widgetclass_t *class = ei_widgetclass_from_name(name);
    ei_widget_t *widget = (class -> allocfunc)();
    widget -> wclass = class;
    widget -> screen_location = hw_surface_get_rect(main_window);
    widget -> pick_color = convert_pick_id_to_pick_color(0);
    ROOT = widget;
    SORTIE = EI_FALSE;
    (ROOT -> wclass -> setdefaultsfunc)(ROOT);
    ROOT -> pick_id = 0;
    WIN_MOVE = malloc(sizeof(ei_point_t));
    WIN_MOVE -> x = 0;
    WIN_MOVE -> y = 0;
    SURFACE_ROOT = main_window;
    SURFACE_PICK = pick_surface;
    WIN_RESIZ = malloc(sizeof(ei_point_t));
    WIN_RESIZ -> x = 0;
    WIN_RESIZ -> y = 0;
    ei_event_set_active_widget(NULL);
    ei_place(ROOT, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    DRAW_RECT = NULL;
}

/**
 * \brief	Releases all the resources of the application, and releases the hardware
 *		(ie. calls \ref hw_quit).
 */


void ei_app_free(){
    free_widgets(ei_app_root_widget ());
    hw_quit();

}

/*
 * \brief Frees all widgets from widget.
 *
 * \param   widget  the widget from which to free.
 */

void free_widgets(ei_widget_t* widget){
    while (widget != NULL){
        (*(widget -> wclass) ->  releasefunc)(widget);
        free_widgets(widget -> children_head);
        widget = widget -> next_sibling;
    }
}

/**
 * \brief	Runs the application: enters the main event loop. Exits when
 *		\ref ei_app_quit_request is called.
 */
void ei_app_run(){
    hw_surface_lock(ei_app_root_surface());
    hw_surface_lock(SURFACE_PICK);
    draw_widgets(ei_app_root_widget());
    hw_surface_unlock(SURFACE_PICK);
    hw_surface_update_rects(SURFACE_PICK, NULL);
    hw_surface_unlock(ei_app_root_surface());
    hw_surface_update_rects(ei_app_root_surface(), NULL);
    DRAW_RECT = NULL;
    ei_event_t event;
    event.type = ei_ev_none;
    while (SORTIE == EI_FALSE){
        hw_event_wait_next(&event);
        ei_point_t where;
        ei_widget_t* widget;
        ei_default_handle_func_t def_func = ei_event_get_default_handle_func();
        if(event.type == ei_ev_mouse_buttondown){
            where = event.param.mouse.where;
            widget = ei_widget_pick(&where);
            if (widget != NULL) {
                (widget -> wclass -> handlefunc)(widget, &event);
            }
            else{
                if (def_func != NULL){
                    if (def_func(&event) == EI_TRUE) {
                        hw_surface_lock(ei_app_root_surface());
                        hw_surface_lock(SURFACE_PICK);
                        draw_widgets(ei_app_root_widget());
                        hw_surface_unlock(SURFACE_PICK);
                        hw_surface_update_rects(SURFACE_PICK, NULL);
                        hw_surface_unlock(ei_app_root_surface());
                        hw_surface_update_rects(ei_app_root_surface(), DRAW_RECT);
                    }
                }
            }
        }
        if (event.type == ei_ev_mouse_buttonup) {
            widget = ei_event_get_active_widget();
            if (widget != NULL) {
                (widget -> wclass -> handlefunc)(widget, &event);
            }
            else{
                if (def_func != NULL){
                    if (def_func(&event) == EI_TRUE) {
                        hw_surface_lock(ei_app_root_surface());
                        hw_surface_lock(SURFACE_PICK);
                        draw_widgets(ei_app_root_widget());
                        hw_surface_unlock(SURFACE_PICK);
                        hw_surface_update_rects(SURFACE_PICK, NULL);
                        hw_surface_unlock(ei_app_root_surface());
                        hw_surface_update_rects(ei_app_root_surface(), DRAW_RECT);
                    }       }
            }
        }
        if (event.type == ei_ev_mouse_move) {
            widget = ei_event_get_active_widget();
            if (widget != NULL) {
                (widget -> wclass -> handlefunc)(widget, &event);
            }
            else{
                if (def_func != NULL){
                    if (def_func(&event) == EI_TRUE) {
                        hw_surface_lock(ei_app_root_surface());
                        hw_surface_lock(SURFACE_PICK);
                        draw_widgets(ei_app_root_widget());
                        hw_surface_unlock(SURFACE_PICK);
                        hw_surface_update_rects(SURFACE_PICK, NULL);
                        hw_surface_unlock(ei_app_root_surface());
                        hw_surface_update_rects(ei_app_root_surface(), DRAW_RECT);
                    }       }
            }
        }
        else{
            if (def_func != NULL){
                if (def_func(&event) == EI_TRUE) {
                    hw_surface_lock(ei_app_root_surface());
                    hw_surface_lock(SURFACE_PICK);
                    draw_widgets(ei_app_root_widget());
                    hw_surface_unlock(SURFACE_PICK);
                    hw_surface_update_rects(SURFACE_PICK, NULL);
                    hw_surface_unlock(ei_app_root_surface());
                    hw_surface_update_rects(ei_app_root_surface(), DRAW_RECT);
                }
            }
        }
        if (DRAW_RECT != NULL){
            hw_surface_lock(ei_app_root_surface());
            hw_surface_lock(SURFACE_PICK);
            draw_widgets(ei_app_root_widget());
            hw_surface_unlock(SURFACE_PICK);
            hw_surface_update_rects(SURFACE_PICK, NULL);
            hw_surface_unlock(ei_app_root_surface());
            hw_surface_update_rects(ei_app_root_surface(), NULL);
            DRAW_RECT = NULL;
        }
    }
}


void draw_widgets(ei_widget_t* widget){
    while (widget != NULL){
        ei_rect_t* clipper = NULL;
        if (widget != ei_app_root_widget()){
            ei_placer_run(widget);
        }
        if (DRAW_RECT != NULL) {
            clipper = ei_intersection(&(ei_app_root_widget() -> screen_location), &(widget -> screen_location));
            ei_widget_t* current = widget;
            while (current != ei_app_root_widget()) {
                clipper = ei_intersection(clipper, &(current -> screen_location));
                current = current -> parent;
            }
            clipper = ei_intersection(clipper, &(DRAW_RECT -> rect));
        } else {
            ei_widget_t* current = widget;
            clipper = ei_intersection(&(ei_app_root_widget() -> screen_location), &(widget -> screen_location));
            while (current != ei_app_root_widget()) {
                clipper = ei_intersection(clipper, &(current -> screen_location));
                current = current -> parent;
            }
        }
        (widget -> wclass ->  drawfunc)(widget, ei_app_root_surface(), SURFACE_PICK, clipper);
        draw_widgets(widget -> children_head);
        widget = widget -> next_sibling;
    }
}

void ei_intersection_linked_rect(ei_rect_t* rect1, ei_linked_rect_t* rect2) {
    ei_linked_rect_t* current = rect2;
    while (current != rect2) {
        current -> rect = *(ei_intersection(&(current -> rect), rect1));
        current = current -> next;
    }
}

ei_rect_t* ei_intersection(ei_rect_t* rect1, ei_rect_t* rect2) {
    ei_point_t point1 = rect1 -> top_left;
    ei_point_t point2 = rect2 -> top_left;
    ei_size_t size1 = rect1 -> size;
    ei_size_t size2 = rect2 -> size;
    ei_rect_t* intersection = malloc(sizeof(ei_rect_t));
    intersection -> top_left.x = max(point1.x, point2.x);
    intersection -> top_left.y = max(point1.y, point2.y);
    intersection -> size.width = max(min(size1.width - intersection ->top_left.x + point1.x, size2.width - intersection ->top_left.x + point2.x),0);
    intersection -> size.height = max(min(size1.height - intersection ->top_left.y + point1.y, size2.height - intersection ->top_left.y + point2.y),0);
    return intersection;
}

/**
 * \brief	Adds a rectangle to the list of rectangles that must be updated on screen. The real
 *		update on the screen will be done at the right moment in the main loop.
 *
 * @param	rect		The rectangle to add, expressed in the root window coordinates.
 *				A copy is made, so it is safe to release the rectangle on return.
 */
void ei_app_invalidate_rect(ei_rect_t* rect){

}

/**
 * \brief	Tells the application to quite. Is usually called by an event handler (for example
 *		when pressing the "Escape" key).
 */
void ei_app_quit_request(){
    SORTIE = EI_TRUE;
}

/**
 * \brief	Returns the "root widget" of the application: a "frame" widget that encapsulate the
 *		root window.
 *
 * @return 			The root widget.
 */
ei_widget_t* ei_app_root_widget(){
    return ROOT;
}

/**
 * \brief	Returns the surface of the root window. Used to create surfaces with similar r, g, b
 *		channels.
 *
 * @return 			The surface of the root window.
 */
ei_surface_t ei_app_root_surface(){
    return SURFACE_ROOT;
}
