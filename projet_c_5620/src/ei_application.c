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
    ROOT = *widget;
    SORTIE = EI_FALSE;
    (ROOT.wclass -> setdefaultsfunc)(&ROOT);
    ROOT.pick_id = 0;
    WIN_MOVE = malloc(sizeof(ei_point_t));
    WIN_MOVE -> x = 0;
    WIN_MOVE -> y = 0;
    SURFACE_ROOT = main_window;
    SURFACE_PICK = pick_surface;
    FIRST_DRAW = 0;
    WIN_RESIZ = malloc(sizeof(ei_point_t));
    WIN_RESIZ -> x = 0;
    WIN_RESIZ -> y = 0;
    ei_event_set_active_widget(NULL);
    ei_place(&ROOT, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    DRAW_RECT = calloc(1, sizeof(ei_linked_rect_t));
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
  ei_event_t event;
  event.type = ei_ev_none;
  while (SORTIE == EI_FALSE){
      hw_event_wait_next(&event);
      ei_point_t where;
      ei_widget_t* widget;
      // ei_default_handle_func_t def_func = ei_event_get_default_handle_func();
//      if(event.type == ei_ev_mouse_buttondown || event.type == ei_ev_mouse_buttonup){
//          where = event.param.mouse.where;
//          widget = ei_widget_pick(&where);
//          if (widget != NULL) {
//              (widget -> wclass -> handlefunc)(widget, &event);
//          }
//          else{
//            def_func(&event);
//          }
//      }
//      if (event.type == ei_ev_mouse_move) {
//        widget = ei_event_get_active_widget();
//        widget = NULL;
//        if (widget != NULL) {
//            (widget -> wclass -> handlefunc)(widget, &event);
//        }
//        else{
//          def_func(&event);
//        }
//      }
//      else{
//        def_func(&event);
//      }
//    if (DRAW == EI_TRUE){
//        DRAW = EI_FALSE;
//        draw_widgets(ei_app_root_widget());
//    }
//    }
//}
    //     }

         switch (event.type) {
             case ei_ev_keydown:
                 if (event.param.key.key_sym == SDLK_ESCAPE) {
                     ei_app_quit_request();
                 }
                 break;
             case ei_ev_mouse_buttondown:
                 where = event.param.mouse.where;
                 widget = ei_widget_pick(&where);
                 if (widget != NULL) {
                     (widget -> wclass -> handlefunc)(widget, &event);
                 }
                 break;
             case ei_ev_mouse_buttonup:
                 where = event.param.mouse.where;
                 widget = ei_event_get_active_widget();
                 if (widget != NULL) {
                     (widget -> wclass -> handlefunc)(widget, &event);
                     }
                 break;
             case ei_ev_mouse_move:
                 widget = ei_event_get_active_widget();
                 if (widget != NULL) {
                     (widget -> wclass -> handlefunc)(widget, &event);
                 }
                 break;
             default:
                 break;
         }
     if (DRAW_RECT != NULL){
         draw_widgets(ei_app_root_widget());
         DRAW_RECT = NULL;
     }
     }
}


void draw_widgets(ei_widget_t* widget){
    while (widget != NULL){
        if (widget != ei_app_root_widget()){
            ei_placer_run(widget);
        }
        (widget -> wclass ->  drawfunc)(widget, ei_app_root_surface(), SURFACE_PICK, &(widget -> screen_location));
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
    SORTIE = EI_TRUE;
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
