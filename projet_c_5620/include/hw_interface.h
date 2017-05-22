/**
 * @file	hw_interface.h
 *
 * @brief	Low level interface with the graphic hadware. This interface is based on the SDL
 *		library.
 * 
 *  Created by François Bérard on 30.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 */

#ifndef HW_INTERFACE_H
#define HW_INTERFACE_H

#include <stdint.h>

#include "ei_types.h"

extern const int EI_MOUSEBUTTON_LEFT;
extern const int EI_MOUSEBUTTON_MIDDLE;
extern const int EI_MOUSEBUTTON_RIGHT;

/**
 * @typedef 	ei_surface_t 
 * @brief 	Surface hidden type. A surface represents a 2 dimentional array of pixels
 *		where drawing can be done. The displayed screen itself is represented by a surface,
 *		it is accessed by \ref hw_create_window. Other "offscreen" surfaces can be created
 *		by \ref hw_surface_create.
 */

typedef void* ei_surface_t;



/**
 * @brief	Initialises access to the low-level operating system services.
 */
void hw_init();

/**
 * @brief	Closes the access to the low-level operating system services.
 */
void hw_quit();



/**
 * @brief 	Opens the main graphical window of the application.
 *
 * @param 	size 		Number of horizontal and vertical pixels.
 * @param 	fullScreen 	If true, opens the window in full screen. Otherwise opens a floating
 *				window.
 *
 * @return 			The unlocked drawing surface (see \ref hw_surface_lock). This
 *				surface	should not be freed by calling \ref hw_surface_free, it is
 *				freed when releasing access to the low-level services by calling
 *				\ref hw_quit.
 */

ei_surface_t hw_create_window(ei_size_t* size, const ei_bool_t fullScreen);

/**
 * @brief 	Allocates an off-screen drawing surface.
 *
 * @param 	root 		The root window which channel indices will be used. This insures
 *				that the offscreen uses the same channel indices (Red, Green, Blue,
 *				Alpha) as the root surface.
 * @param 	size 		Number of horizontal and vertical pixels.
 * @param	force_alpha	If true, then the returned surface will use an alpha channel
 *				regardless of root having an alpha channel or not.
 *
 * @return 			The unlocked drawing surface (see \ref hw_surface_lock). The surface
 *				should be freed by calling \ref hw_surface_free.
 */

ei_surface_t hw_surface_create (const ei_surface_t	root,
				const ei_size_t*	size,
				ei_bool_t		force_alpha);

/**
 * @brief 	Frees a surface allocated by \ref hw_surface_create.
 *		This must be called on an unlocked surface (see \ref hw_surface_unlock).
 *
 * @param 	surface		The surface to be freed.
 */

void hw_surface_free(ei_surface_t surface);

/**
 * @brief	Gains exclusive access to a surface.
 * 		Every call to this function must be matched by a call to \ref hw_surface_unlock.
 * 		The address of the pixel buffer may change while the surface is unlocked. Thus,
 *		 \ref hw_surface_get_buffer must called after each call to this function.
 *
 * @param 	surface 	The surface to lock.
 */
void hw_surface_lock(ei_surface_t surface);

/** 
 * @brief 	Releases the exclusive access to a surface that was locked by \ref hw_surface_lock.
 *
 * @param 	surface 	The surface to unlock.
 */
void hw_surface_unlock(ei_surface_t surface);

/**
 * @brief	Requests that a list of rectangular regions of the root surface be updated on
 *		screen.
 *
 * @param	surface 	The surface returned by \ref hw_create_window.
 *				This function can only be called on *unlocked surfaces*
 *				(\ref hw_surface_unlock).
 * @param	rects		The list of rectangle to be updated on screen.
 *				If NULL, them the entire surface is updated.
 */
void hw_surface_update_rects(ei_surface_t surface, const ei_linked_rect_t* rects);

/**
 * @brief 	Returns the R, G, B, and Alpha channel indices of a surface.
 *		For example, if *ir is 2, this means that red is encoded in the third byte
 *		of the 4 byte pixels of this surface.
 *		*ia may be -1, this means that the surface does not handle alpha.
 *
 * @param 	surface		The surface.
 * @param 	ir, ig, ib, ia	Where to store the resulting indices.
 */

void hw_surface_get_channel_indices(ei_surface_t surface, int* ir, int* ig, int* ib, int* ia);

/** 
 * @brief	Sets the coordinates of the first pixel of the surface's memory.
 *		By default, the coordinates of the first pixel are (0, 0). This can be changed by
 *		a call to this function.
 *		After a call to this function, the function \ref hw_surface_get_buffer returns a
 *		different address than before.
 *
 * @param 	surface		The surface which origin must be changed.
 * @param 	origin		The new coordinates of the first pixel of the surface.
 */
void hw_surface_set_origin(ei_surface_t surface, const ei_point_t origin);

/** 
 * @brief	Returns a pointer to the address of the pixel at coordinated (0, 0) of a surface.
 *		This is usually the first pixel of the surface's memory. But after a call to
 * 		\ref hw_surface_set_origin, the (0, 0) pixel may point within the surface memory or not.
 *		Pixels are ordered by horizontal lines,
 *		from top to bottom, and from left to right whitin lines.
 * 		The pixel buffer of a surface may be moved when the surface is unlocked
 *		(\ref hw_surface_unlock), you must thus call this function after each call
 *		to \ref hw_surface_lock.
 *
 * @param 	surface		The surface from which the pixel address is returned.
 */
uint8_t* hw_surface_get_buffer(const ei_surface_t surface);

/**
 * @brief	Returns the size of a surface.
 *
 * @param 	surface		The surface which size is requested.
 *
 * @return			The size of the surface.
 */
ei_size_t hw_surface_get_size(const ei_surface_t surface);

/**
 * @brief	Returns the rectangle of a surface (origin and size).
 *
 * @param 	surface		The surface which rectangle is requested.
 *
 * @return			The rectangle of the surface.
 */
ei_rect_t hw_surface_get_rect(const ei_surface_t surface);

/** 
 * @brief	Tells if a surface manages transparency, i.e. if the surface has an
 *		alpha channel.
 *
 * @return	A boolean: \ref EI_TRUE means that the surface has an alpha (transparency) channel,
 *		\ref EI_FALSE means it does not.
 */
ei_bool_t hw_surface_has_alpha(ei_surface_t surface);

/**
 * @brief	Creates a font that can be used to render text. The font must be freed by calling
 *		\ref hw_text_font_free.
 *
 * @param 	filename	The path to the file containing the ttf font definition. Can be
 *				relative.
 * @param 	style		The style of the font (normal, bold, ...).
 * @param 	size		The size of the characters in pixels.
 *
 * @return			The font.
 */
ei_font_t hw_text_font_create		(const char*		filename,
					 ei_fontstyle_t		style,
					 int			size);
/**
 * @brief	Frees a font created by \ref hw_text_font_create.
 *
 * @param	font		The font to be freed.
 */
void hw_text_font_free(ei_font_t font);

/**
 * @brief	Computes the size of a text surface givent the font and the text.
 *
 * @param	text		The string of the message.
 * @param	font		The font used to render the text.
 * @param	width, height	Addresses where to store the computed width and height of the text
 *				surface.
 */
void hw_text_compute_size		(const char*		text,
					 const ei_font_t	font,
					 int*			width,
					 int*			height);

/**
 * @brief	Creates a surface containing a text. The size of the created surface is just big
 *		enough to contain the text. The caller is responsible to release this surface
 *		(\ref hw_surface_free) when it is no more needed.
 *
 * @param	text		The string of the message.
 * @param	font		The font used to render the text.
 * @param	color		The text color. The alpha parameter is not used. However, the text
 *				is rendered with alpha blending to smooth the curves of the letters
 *				(anti-aliasing).
 *
 * @return			A newly created unlocked surface containing an anti-aliased
 *				rendering of the text. The anti-aliasing is implemented with the
 *				alpha channel of the surface: pixels on the text's boundaries have
 *				some transparency.
 */
ei_surface_t hw_text_create_surface	(const char*		text,
					 const ei_font_t	font,
					 const ei_color_t*	color);



/**
 * @brief	Creates a surface and loads into it an image read from a file. The caller is
 *		responsible to release this surface (\ref hw_surface_free) when it is no more
 *		needed.
 *
 * @param	filename	The name of the file containing the image. The file can be .png,
 *				.tiff, .jpg, etc.
 * @param	channels	A surface to define channel ordering: the newly created surface that
 *				is returned by this function will have the same channel order as
 *				this surface.
 *
 * @return			A new unlocked surface containing the image.
 */
ei_surface_t hw_image_load		(const char*		filename,
					 ei_surface_t		channels);



struct ei_event_t;

/**
 * @brief	Lets this processus sleep until a new event is available.
 *
 * @param	event		Where to store the new event. The structure must be allocated by the
 *				caller.	On return, the structure is filled with informations about
 *				the new event.
 */
void hw_event_wait_next(struct ei_event_t* event);



/**
 * @brief	Put an application-generated event on the event queue. This will cause
 *		\ref hw_event_wait_next to wake.
 *
 * @param	user_param	The user parameter that will be retrievable in the event.
 */
int hw_event_post_app(void* user_param);



/**
 * @brief	Schedule an application-generated event to be posted after some amount of time.
 *		This will cause \ref hw_event_wait_next to wake after this amount of time.
 *
 * @param	ms_delay	The amount of time, in milliseconds, to wait before the event is
 *				posted in the event queue.
 * @param	user_param	The user parameter that will be retrievable in the event.
 */
void hw_event_schedule_app(int ms_delay, void* user_param);



/**
 * @brief	Returns the current time, in seconds, from some arbitraty origin. Can be used to
 *		measure elpased time between to calls.
 *
 * @return			The current time, in seconds.
 */
double hw_now();


#endif
