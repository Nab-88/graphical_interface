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
    ei_frame_register_class();
    ei_widgetclass_name_t name;
    strncpy(name, "frame", 20);
    ei_widgetclass_t *class = ei_widgetclass_from_name(name);
    ei_widget_t *widget = (class -> allocfunc)();
    widget -> wclass = class;
    widget -> screen_location = hw_surface_get_rect(main_window);
    ROOT = *widget;
    SURFACE_ROOT = main_window;

}

/**
 * \brief	Releases all the resources of the application, and releases the hardware
 *		(ie. calls \ref hw_quit).
 */


void ei_app_free(){
    free_widgets(ei_app_root_widget () -> children_head);
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
    draw_widgets(ei_app_root_widget ());
    getchar();
}

void draw_widgets(ei_widget_t* widget){
    while (widget != NULL){
        (*(widget -> wclass) ->  drawfunc)(widget, ei_app_root_surface(), NULL, &(widget -> screen_location));
        draw_widgets(widget -> children_head);
        widget = widget -> next_sibling;
    }
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

}

/**
 * \brief	Returns the "root widget" of the application: a "frame" widget that encapsulate the
 *		root window.
 *
 * @return 			The root widget.
 */
ei_widget_t* ei_app_root_widget(){
    return &ROOT;
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
