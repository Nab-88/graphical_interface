#ifndef EI_DRAW_EXTENSION_H
#define EI_DRAW_EXTENSION_H

#include "ei_types.h"


void copy_pixel(uint32_t* dest_pixel, uint32_t* src_pixel, ei_surface_t src_surf,  ei_surface_t dest_surf);

/**
 * \brief	Draws a scanline
 *
 * @param	surface 	Where to draw the polygon. The surface must be *locked* by
 *				\ref hw_surface_lock.
 * @param	table of active sides
 * @param	color		The color used to draw the polygon, alpha channel is managed.
 */

ei_bool_t pixel_is_in_rect(ei_point_t pixel, const ei_rect_t* rect);



/**
 * \brief	copy the src_rect of dst_rect pixel by pixel using the course formula
 *
 * @param	dst_rect the rectangle which delimits the destination of the copy
 * @param	src_rect the rectangle which delimits the source of the copy
 * @param	destination the surface destination of the copy
 * @param	source the surface source of the copu
 * @param alpha true or false if the copy use the alpha parameters (see ei_copy_surface)
 */
void ei_copy2(const ei_rect_t* dst_rect, const ei_rect_t* src_rect, const ei_surface_t destination, const ei_surface_t source, const ei_bool_t alpha);

#endif
