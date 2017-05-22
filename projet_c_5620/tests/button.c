#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"


/*
 * button_press --
 *
 *	Callback called when a user clicks on the button.
 */
void button_press(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	printf("Click !\n");
}

/*
 * process_key --
 *
 *	Callback called when any key is pressed by the user.
 *	Simply looks for the "Escape" key to request the application to quit.
 */
ei_bool_t process_key(ei_event_t* event)
{
	if (event->type == ei_ev_keydown)
		if (event->param.key.key_sym == SDLK_ESCAPE) {
			ei_app_quit_request();
			return EI_TRUE;
		}

	return EI_FALSE;
}

/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_widget_t*	button;
	ei_size_t	button_size		= {300,200};
	int		button_corner_radius	= 40;
	int		button_x		= 150;
	int		button_y		= 200;
	ei_color_t	button_color		= {0x88, 0x88, 0x88, 0xff};
	char*		button_title		= "Mon premier Bouton !";
	ei_color_t	button_text_color	= {0x00, 0x00, 0x00, 0xff};
	ei_relief_t	button_relief		= ei_relief_raised;
	int		button_border_width	= 6;
	ei_callback_t	button_callback 	= button_press;

	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	ei_event_set_default_handle_func(process_key);

	/* Create, configure and place the button on screen. */
	button = ei_widget_create("button", ei_app_root_widget());
	ei_button_configure	(button, &button_size, &button_color,
				 &button_border_width, &button_corner_radius, &button_relief, &button_title, NULL, &button_text_color, NULL,
				 NULL, NULL, NULL, &button_callback, NULL);
	ei_place(button, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );

	/* Hook the keypress callback to the event. */
	ei_frame_configure(ei_app_root_widget(), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
