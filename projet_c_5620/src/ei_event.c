#include "ei_event.h"
#include <stdlib.h>
#include "ei_all_widgets.h"
#include "ei_application.h"
/**
 *  @file	ei_event.c
 *  @brief	Allows the binding and unbinding of callbacks to events.
 *
 *  \author
 *  Created by François Bérard on 30.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */

/**
 * Sets the widget which is currently being manipulated by the user.
 *
 * @param	widget		The widget to declare as active, or NULL to declare
 *				that a widget is no more being manipulated.
 */
void ei_event_set_active_widget(ei_widget_t* widget){
    EVENT_ACTIVE = widget;
    if (widget != NULL && widget != ei_app_root_widget()){
        ei_widget_t* parent = (widget -> parent);
        ei_widget_t* previous = ei_widget_previous(widget);
        if (previous == NULL && widget != parent -> children_tail){
            parent -> children_head = widget -> next_sibling;
        } else if ( widget == parent -> children_tail) {
            return;
        } else {
            previous -> next_sibling = widget -> next_sibling;
        }
        parent -> children_tail -> next_sibling = widget;
        parent -> children_tail = widget;
        widget -> next_sibling = NULL;
    }
}

/**
 * Returns the widget currently being manipulated by the user.
 *
 * @return			The widget currenlty being manipulated, or NULL.
 */
ei_widget_t* ei_event_get_active_widget(){
    return EVENT_ACTIVE;
}


/**
 * Sets the function that must be called when an event has been received but no processed
 *	by any widget.
 *
 * @param	func		The event handling function.
 */
void ei_event_set_default_handle_func(ei_default_handle_func_t func){
  DEF_FUNC = func;
}

/**
 * Returns the function that must be called when an event has been received but no processed
 *	by any widget.
 *
 * @return			The address of the event handling function.
 */
ei_default_handle_func_t ei_event_get_default_handle_func(){
  return DEF_FUNC;
}
