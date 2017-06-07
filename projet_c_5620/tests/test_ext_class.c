//
//  test_ext_class.c
//  projet_c_IG
//
//  Created by François Bérard on 1/6/16.
//  Copyright © 2016 LIG-IIHM. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "ext_testclass.h"

#include "ei_utils.h"
#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"




// global_event_handler --

ei_bool_t global_event_handler(ei_event_t* event)
{
	if (event->param.key.key_sym == SDLK_ESCAPE) {
		ei_app_quit_request();
		return EI_TRUE;
	}
        return EI_FALSE;
}

// button_callback --
//
//	The function called when the internal button is clicked.

void button_callback	(ei_widget_t*		widget,
			 struct ei_event_t*	event,
			 void*			user_param)
{
	printf("Click on button.\n");
}


// Uncomment either one of the two following lines.
//	If true, the testclass drawfunc draws a widget *and its children*.
//	If false, the testclass drawfunc only draws the widget, not its children.

 static const ei_bool_t	k_drawfunc_draws_children	= EI_TRUE;
// static const ei_bool_t	k_drawfunc_draws_children	= EI_FALSE;



// ei_main --

int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size		= {800, 800};
        ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};
	ei_anchor_t	anchor_c		= ei_anc_center;
	float		fl_1_0			= 1.0;
	float		fl_0_5			= 0.5;

        ei_widget_t*	top;
        ei_size_t	top_size		= {400,400};
        char*		top_title		= "Test nouvelle classe de widget";
        ei_color_t	top_color		= {0xA0,0xA0,0xA0, 0xff};
        int		top_border_width	= 2;
        ei_bool_t	top_closable		= EI_TRUE;
        ei_axis_set_t	top_resizable		= ei_axis_both;
        ei_point_t	top_position		= {30, 10};
        ei_size_t	top_min_size		= {50, 50};
        ei_size_t*	top_min_size_ptr	= &top_min_size;

        ei_widget_t*	test;
	ei_size_t	test_sz;

	ei_widget_t*	but;
	char*		but_text		= "Click me !";
	ei_callback_t	but_callback		= button_callback;

	// Initialize the application, set the background color.

	ei_app_create(&screen_size, EI_FALSE);
        ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	// Register the new "testclass" widget class.

	ext_testclass_register_class(k_drawfunc_draws_children); // See k_drawfunc_draws_children comment above.

	// Create a toplevel, place it on the screen.

        top = ei_widget_create("toplevel", ei_app_root_widget());
        ei_toplevel_configure(top, &top_size, &top_color, &top_border_width, &top_title,
								&top_closable, &top_resizable, &top_min_size_ptr);
        ei_place(top, NULL, &(top_position.x), &(top_position.y), NULL, NULL, NULL, NULL, NULL, NULL);

	// Create the testclass widget as a child of the toplevel, use relative placement.

	test = ei_widget_create("testclass", top);
	ext_testclass_configure(test, 8);
	test_sz		= ei_size(-40, -40);
	ei_place(test, &anchor_c, NULL, NULL, &(test_sz.width), &(test_sz.height), &fl_0_5, &fl_0_5, &fl_1_0, &fl_1_0);

	// Create a button as a child of the testclass widget, place it.

	but = ei_widget_create("button", test);
	ei_button_configure(but, NULL, NULL, NULL, NULL, NULL, &but_text, NULL, NULL, NULL, NULL, NULL, NULL, &but_callback, NULL);
	ei_place(but, &anchor_c, NULL, NULL, NULL, NULL, &fl_1_0, &fl_0_5, NULL, NULL);

	// Run the main loop, bind the "Esc" key to quitting the application.

	ei_event_set_default_handle_func(global_event_handler);

        ei_app_run();

	ei_app_free();
	return (EXIT_SUCCESS);
}
